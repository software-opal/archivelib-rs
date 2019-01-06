use std::io;

#[derive(Fail, Debug)]
pub enum ReadError {
  #[fail(display = "End of file")]
  EndOfFile(),
  #[fail(display = "IO error: {}", error)]
  IoError {
    #[cause]
    error: io::Error,
  },
}
impl From<io::Error> for ReadError {
  fn from(err: io::Error) -> ReadError {
    ReadError::IoError { error: err }
  }
}
impl PartialEq for ReadError {
  fn eq(&self, rhs: &ReadError) -> bool {
    match (self, rhs) {
      (ReadError::EndOfFile(), ReadError::EndOfFile()) => true,
      _ => false,
    }
  }
}

type Result<T> = std::result::Result<T, ReadError>;

pub trait BitwiseRead {
  fn try_read_bits(&mut self, bits: usize) -> Result<(u8, usize)>;

  fn read_bit(&mut self) -> Result<bool> {
    match self.try_read_bits(1)? {
      (bit, 1) => Ok(bit != 0),
      _ => panic!("Invariant of `try_read_bytes` failed"),
    }
  }
  fn read_bits(&mut self, bits: usize) -> Result<u128> {
    if bits == 0 {
      return Ok(0);
    } else if bits >= 128 {
      panic!("Cannot read more than 128 bits");
    }
    let mut out: u128 = 0;
    let mut shift = bits;
    while shift > 0 {
      match self.try_read_bits(shift)? {
        (_, 0) => panic!("Invariant of `try_read_bytes` failed"),
        (bytes, bits_read) => {
          shift -= bits_read;
          out |= (bytes as u128) << shift;
        }
      }
    }
    Ok(out)
  }
  fn read_byte(&mut self) -> Result<u8> {
    Ok(self.read_bits(8)? as u8)
  }
}

pub struct BitwiseReader<R: io::Read> {
  inner: R,
  pending_byte: u8,
  index: usize,
}

impl<R: io::Read> BitwiseReader<R> {
  pub fn new(inner: R) -> BitwiseReader<R>
  where
    R: io::Read,
  {
    BitwiseReader {
      pending_byte: 0,
      index: 0,
      inner,
    }
  }

  fn read_next_into_pending(&mut self) -> Result<()> {
    let mut tmp = [0; 1];
    if self.inner.read(&mut tmp)? != 1 {
      self.index = 0;
      return Err(ReadError::EndOfFile());
    }
    self.index = 8;
    self.pending_byte = tmp[0];
    Ok(())
  }
}

impl<R: io::Read> BitwiseRead for BitwiseReader<R> {
  fn try_read_bits(&mut self, bits: usize) -> Result<(u8, usize)> {
    if bits == 0 {
      return Ok((0, 0));
    }
    if self.index == 0 {
      self.read_next_into_pending()?;
    }
    let (byte, pos) = (self.pending_byte, self.index);
    assert!(pos > 0 && pos <= 8);
    // Mask out all bits higher than pos
    let mask = if pos == 8 {
      0xff
    } else {
      // Enable all bits upto & including pos
      (0x1 << pos) - 1
    };
    let usable_byte = byte & mask;
    if bits >= pos {
      self.index = 0;
      Ok((usable_byte, pos))
    } else {
      let new_pos = pos - bits;
      self.index = new_pos;
      Ok((usable_byte >> new_pos, bits))
    }
  }
}

pub struct VecReader {
  data: Vec<u8>,
  index: usize,
}
impl VecReader {
  pub fn new(data: Vec<u8>) -> Self {
    VecReader { data, index: 0 }
  }
}
impl io::Read for VecReader {
  fn read(&mut self, buf: &mut [u8]) -> io::Result<usize> {
    let amt = buf.len().min(self.data.len() - self.index);
    let end = self.index + amt;
    buf[..amt].copy_from_slice(&self.data[self.index..end]);

    self.index = end;
    Ok(amt)
  }
}
impl From<Vec<u8>> for VecReader {
  fn from(data: Vec<u8>) -> Self {
    VecReader::new(data)
  }
}

#[cfg(test)]
mod tests {
  use super::*;

  mod try_read_bits {
    use super::{BitwiseRead, *};
    #[test]
    fn test_empty() {
      let data: Vec<u8> = vec![];
      let mut reader = BitwiseReader::new(VecReader::new(data));
      assert_eq!(reader.try_read_bits(1), Err(ReadError::EndOfFile()));
    }
    #[test]
    fn test_loads_data() {
      let data = vec![0xFA, 0xCE];
      let mut reader = BitwiseReader::new(VecReader::new(data));
      assert_eq!(reader.try_read_bits(0), Ok((0, 0)));
      assert_eq!(reader.try_read_bits(2), Ok((0x3, 2)));
      assert_eq!(reader.try_read_bits(100), Ok((0x3A, 6)));
      assert_eq!(reader.try_read_bits(100), Ok((0xCE, 8)));
    }
  }

  mod read_byte {
    use super::*;

    #[test]
    fn test_empty() {
      let data = vec![];
      let mut reader = BitwiseReader::new(VecReader::new(data));
      assert_eq!(reader.read_byte(), Err(ReadError::EndOfFile()));
    }
    #[test]
    fn test_1_byte() {
      let data = vec![0xF0];
      let mut reader = BitwiseReader::new(VecReader::new(data));
      assert_eq!(reader.read_byte(), Ok(0xF0));
      assert_eq!(reader.read_byte(), Err(ReadError::EndOfFile()));
    }
    #[test]
    fn test_unaligned_read_byte() {
      let data = vec![0xF0, 0xAB];
      let mut reader = BitwiseReader::new(VecReader::new(data));
      assert_eq!(reader.read_bits(4), Ok(0xF));
      assert_eq!(reader.read_byte(), Ok(0x0A));
      assert_eq!(reader.read_byte(), Err(ReadError::EndOfFile()));
    }
    #[test]
    fn test_aligned_read_then_unaligned_byte() {
      let data = vec![0xF0, 0xAB];
      let mut reader = BitwiseReader::new(VecReader::new(data));
      assert_eq!(reader.read_byte(), Ok(0xF0));
      assert_eq!(reader.read_bits(4), Ok(0xA));
      assert_eq!(reader.read_byte(), Err(ReadError::EndOfFile()));
    }
  }
  mod read_bits {
    use super::*;
    #[test]
    fn test_small_unaligned_to_aligned_read() {
      let data = vec![0b1100_1010, 0b1110_0010];
      let mut reader = BitwiseReader::new(VecReader::new(data));
      assert_eq!(reader.read_bits(3), Ok(0b110));
      assert_eq!(reader.read_bits(13), Ok(0b0_1010_1110_0010));
      assert_eq!(reader.read_bit(), Err(ReadError::EndOfFile()));
    }
    #[test]
    fn test_small_unaligned_read() {
      let data = vec![0b1100_1010, 0b1110_0010];
      let mut reader = BitwiseReader::new(VecReader::new(data));
      assert_eq!(reader.read_bits(3), Ok(0b110));
      assert_eq!(reader.read_bits(12), Ok(0b0_1010_1110_001));
      assert_eq!(reader.read_bits(1), Ok(0b0));
      assert_eq!(reader.read_bit(), Err(ReadError::EndOfFile()));
    }
    #[test]
    fn test_unaligned_read_byte() {
      let data = vec![0xF0, 0xAB];
      let mut reader = BitwiseReader::new(VecReader::new(data));
      assert_eq!(reader.read_bits(3), Ok(0x7));
      assert_eq!(reader.read_bits(0), Ok(0x0));
      assert_eq!(reader.read_bits(1), Ok(0x1));
      assert_eq!(reader.read_bits(0), Ok(0x0));
      assert_eq!(reader.read_bits(8), Ok(0x0A));
    }
    #[test]
    fn test_unaligned_large_read_byte() {
      let data = vec![0xF0, 0xAB, 0xCD, 0xEF];
      let mut reader = BitwiseReader::new(VecReader::new(data));
      assert_eq!(reader.read_bits(4), Ok(0xF));
      assert_eq!(reader.read_bits(24), Ok(0x0ABCDE));
      assert_eq!(reader.read_bits(2), Ok(0x3));
      assert_eq!(reader.read_bits(3), Err(ReadError::EndOfFile()));
    }
    #[test]
    fn test_unaligned_large_uneven_read_byte() {
      let data = vec![0xF0, 0xAB, 0xCD, 0b1110_1111];
      let mut reader = BitwiseReader::new(VecReader::new(data));
      assert_eq!(reader.read_bits(4), Ok(0xF));
      assert_eq!(reader.read_bits(20), Ok(0x0ABCD));
      assert_eq!(reader.read_bits(6), Ok(0b1110_11));
      assert_eq!(reader.read_bits(3), Err(ReadError::EndOfFile()));
    }
  }
  mod read_bit {
    use super::*;

    #[test]
    fn test_1_byte_bitwise() {
      let data = vec![0x8F];
      let mut reader = BitwiseReader::new(VecReader::new(data));
      assert_eq!(reader.read_bit(), Ok(true));
      assert_eq!(reader.read_bit(), Ok(false));
      assert_eq!(reader.read_bit(), Ok(false));
      assert_eq!(reader.read_bit(), Ok(false));
      assert_eq!(reader.read_bit(), Ok(true));
      assert_eq!(reader.read_bit(), Ok(true));
      assert_eq!(reader.read_bit(), Ok(true));
      assert_eq!(reader.read_bit(), Ok(true));
      assert_eq!(reader.read_byte(), Err(ReadError::EndOfFile()));
    }
  }
}
