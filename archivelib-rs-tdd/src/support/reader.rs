use std::fmt;
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
    (ReadError::  EndOfFile(), ReadError::  EndOfFile()) => true
    ,_=> false
    }
  }
}


type Result<T> = std::result::Result<T, ReadError>;

pub struct Reader<R> {
  pending_bit: Option<(u8, usize)>,
  inner: R,
}

impl<R: io::Read> Reader<R> {
  pub fn new(data: R) -> Self {
    Reader {
      pending_bit: None,
      inner: data,
    }
  }

  fn get_pending_bit(&mut self) -> Result<(u8, usize)> {
    let result = match self.pending_bit {
      Some((byte, pos)) => (byte, pos),
      None => {
        let mut tmp = [0; 1];
        match self.inner.read(&mut tmp)? {
          1 => (tmp[0], 8),
          _ => return Err(ReadError::EndOfFile()),
        }
      }
    };
    self.pending_bit = Some(result);
    Ok(result)
  }

  pub fn read_bit(&mut self) -> Result<bool> {
    let (present_byte, present_pos) = self.get_pending_bit()?;
    let new_pos = present_pos - 1;
    self.pending_bit = if new_pos <= 0 {
      None
    } else {
      Some((present_byte, new_pos))
    };
    Ok(0 != (present_byte & (1 << new_pos)))
  }

  pub fn read_bits(&mut self, bits: usize) -> Result<u128> {
    assert!(bits <= 128);

    let mut out: u128 = 0;
    let mut shift = bits;
    let (_, pos) = self.get_pending_bit()?;
    let pre_bits = bits.min(pos % 8);
    let post_bits = (bits - pre_bits) % 8;
    let whole_bits = (bits - pre_bits - post_bits) / 8;

    for _ in 0..pre_bits {
      shift -= 1;
      if self.read_bit()? {
        out |= 1 << shift
      }
    }
    for _ in 0..whole_bits {
      shift -= 8;
      match self.get_pending_bit()? {
        (value, 8) => {
          out |= (value as u128) << shift;
          self.pending_bit = None;
        }
        (_, _) => {
          panic!("Pending bit unaligned");
        }
      }
    }
    for _ in 0..post_bits {
      shift -= 1;
      if self.read_bit()? {
        out |= 1 << shift
      }
    }
    return Ok(out);
  }

  pub fn read_byte(&mut self) -> Result<u8> {
    match self.get_pending_bit()? {
      (val, 8) => {
        self.pending_bit = None;
        Ok(val)
      }
      _ => Ok(self.read_bits(8)? as u8),
    }
  }
}

impl<R> fmt::Debug for Reader<R> {
  fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
    write!(
      f,
      "Reader{{ pending_bit: {:?}, data: (...) }}",
      self.pending_bit
    )
  }
}

#[cfg(test)]
mod tests {
  use super::*;

  mod read_byte {
    use super::*;

    #[test]
    fn test_empty() {
      let data: &[u8] = &vec![];
      let mut reader = Reader::new(data);
      assert_eq!(reader.read_byte(), Err(ReadError::EndOfFile()));
    }
    #[test]
    fn test_1_byte() {
      let data: &[u8] = &vec![0xF0];
      let mut reader = Reader::new(data);
      assert_eq!(reader.read_byte(), Ok(0xF0));
      assert_eq!(reader.read_byte(), Err(ReadError::EndOfFile()));
    }
    #[test]
    fn test_unaligned_read_byte() {
      let data: &[u8] = &vec![0xF0, 0xAB];
      let mut reader = Reader::new(data);
      assert_eq!(reader.read_bits(4), Ok(0xF));
      assert_eq!(reader.read_byte(), Ok(0x0A));
      assert_eq!(reader.read_byte(), Err(ReadError::EndOfFile()));
    }
    #[test]
    fn test_aligned_read_then_unaligned_byte() {
      let data: &[u8] = &vec![0xF0, 0xAB];
      let mut reader = Reader::new(data);
      assert_eq!(reader.read_byte(), Ok(0xF0));
      assert_eq!(reader.read_bits(4), Ok(0xA));
      assert_eq!(reader.read_byte(), Err(ReadError::EndOfFile()));
    }
  }

  mod read_bits {
    use super::*;
    #[test]
    fn test_small_unaligned_to_aligned_read() {
      let data: &[u8] = &vec![0b1100_1010, 0b1110_0010];
      let mut reader = Reader::new(data);
      assert_eq!(reader.read_bits(3), Ok(0b110));
      assert_eq!(reader.read_bits(13), Ok(0b0_1010_1110_0010));
      assert_eq!(reader.read_bit(), Err(ReadError::EndOfFile()));
    }
    #[test]
    fn test_small_unaligned_read() {
      let data: &[u8] = &vec![0b1100_1010, 0b1110_0010];
      let mut reader = Reader::new(data);
      assert_eq!(reader.read_bits(3), Ok(0b110));
      assert_eq!(reader.read_bits(12), Ok(0b0_1010_1110_001));
      assert_eq!(reader.read_bits(1), Ok(0b0));
      assert_eq!(reader.read_bit(), Err(ReadError::EndOfFile()));
    }
  }

  #[test]
  fn test_unaligned_read_byte() {
    let data: &[u8] = &vec![0xF0, 0xAB];
    let mut reader = Reader::new(data);
    assert_eq!(reader.read_bits(3), Ok(0x7));
    assert_eq!(reader.read_bits(0), Ok(0x0));
    assert_eq!(reader.read_bits(1), Ok(0x1));
    assert_eq!(reader.read_bits(0), Ok(0x0));
    assert_eq!(reader.read_bits(8), Ok(0x0A));
  }
  #[test]
  fn test_unaligned_large_read_byte() {
    let data: &[u8] = &vec![0xF0, 0xAB, 0xCD, 0xEF];
    let mut reader = Reader::new(data);
    assert_eq!(reader.read_bits(4), Ok(0xF));
    assert_eq!(reader.read_bits(24), Ok(0x0ABCDE));
    assert_eq!(reader.read_bits(2), Ok(0x3));
    assert_eq!(reader.read_bits(3), Err(ReadError::EndOfFile()));
  }
  #[test]
  fn test_unaligned_large_uneven_read_byte() {
    let data: &[u8] = &vec![0xF0, 0xAB, 0xCD, 0b1110_1111];
    let mut reader = Reader::new(data);
    assert_eq!(reader.read_bits(4), Ok(0xF));
    assert_eq!(reader.read_bits(20), Ok(0x0ABCD));
    assert_eq!(reader.read_bits(6), Ok(0b1110_11));
    assert_eq!(reader.read_bits(3), Err(ReadError::EndOfFile()));
  }
  mod read_bit {
    use super::*;

    #[test]
    fn test_1_byte_bitwise() {
      let data: &[u8] = &vec![0x8F];
      let mut reader = Reader::new(data);
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
