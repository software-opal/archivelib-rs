use super::base::{LookAheadBitwiseRead, LookAheadBitwiseReader};

pub trait CorrectLookAheadBitwiseRead: LookAheadBitwiseRead {
  fn is_eof(&self) -> bool;
  fn eof_bits(&self) -> usize;
  fn consume_bits_nopad(&mut self, bits: usize) -> std::io::Result<Vec<bool>>;
  fn look_ahead_bits_nopad(&mut self, bits: usize) -> std::io::Result<Vec<bool>>;
}
pub struct CorrectLookAheadBitwiseReader<R: LookAheadBitwiseRead> {
  reader: R,
  eof_bits: usize,
  buffer: [bool; 16],
}

impl<R: LookAheadBitwiseRead> CorrectLookAheadBitwiseReader<R> {
  pub fn new(reader: R) -> Self {
    CorrectLookAheadBitwiseReader {
      reader,
      eof_bits: 0,
      buffer: [false; 16],
    }
  }
  fn pad_buffer(&self, bits: usize, buffer: &mut Vec<bool>) -> usize {
    if buffer.len() == bits {
      return 0;
    }
    assert!(buffer.len() < bits);
    if buffer.capacity() < bits {
      buffer.reserve(bits - buffer.capacity());
    }
    while buffer.len() + self.buffer.len() <= bits {
      buffer.extend_from_slice(&self.buffer);
    }
    assert!(bits - buffer.len() < self.buffer.len());
    if buffer.len() < bits {
      let remaining = bits - buffer.len();
      buffer.extend_from_slice(&self.buffer[..remaining]);
      return remaining;
    }
    assert_eq!(buffer.len(), bits);
    return 0;
  }
}

impl<I: std::io::Read> CorrectLookAheadBitwiseReader<LookAheadBitwiseReader<I>> {
  pub fn from_reader(reader: I) -> Self {
    CorrectLookAheadBitwiseReader::new(LookAheadBitwiseReader::new(reader))
  }
}

impl<R: LookAheadBitwiseRead> From<R> for CorrectLookAheadBitwiseReader<R> {
  fn from(reader: R) -> CorrectLookAheadBitwiseReader<R> {
    Self::new(reader)
  }
}

impl<R: LookAheadBitwiseRead> CorrectLookAheadBitwiseRead for CorrectLookAheadBitwiseReader<R> {
  fn is_eof(&self) -> bool {
    self.eof_bits > 0
  }
  fn eof_bits(&self) -> usize {
    self.eof_bits
  }
  fn consume_bits_nopad(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    if bits == 0 {
      return Ok(vec![]);
    }
    let consumed = self.reader.consume_bits(bits)?;
    assert!(consumed.len() <= bits);
    if consumed.len() >= self.buffer.len() {
      let start = consumed.len() - self.buffer.len();
      self.buffer.copy_from_slice(&consumed[start..]);
    } else if consumed.len() > 0 {
      let start = self.buffer.len() - consumed.len();
      self.buffer.rotate_left(consumed.len());
      self.buffer[start..].copy_from_slice(&consumed[..]);
    }
    Ok(consumed)
  }
  fn look_ahead_bits_nopad(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    self.reader.look_ahead_bits(bits)
  }
}
impl<R: LookAheadBitwiseRead> LookAheadBitwiseRead for CorrectLookAheadBitwiseReader<R> {
  fn consume_bits(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    let mut consumed = self.consume_bits_nopad(bits)?;
    assert!(consumed.len() <= bits);
    if bits != consumed.len() {
      self.eof_bits += bits - consumed.len();
      let to_shift = self.pad_buffer(bits, &mut consumed);
      self.buffer.rotate_left(to_shift);
    }
    assert_eq!(consumed.len(), bits);
    Ok(consumed)
  }
  fn look_ahead_bits(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    let mut lookahead = self.look_ahead_bits_nopad(bits)?;
    self.pad_buffer(bits, &mut lookahead);
    Ok(lookahead)
  }
}

#[cfg(test)]
mod tests {
  use super::*;
  use crate::support::{LookAheadBitwiseRead, LookAheadBitwiseReader};

  #[test]
  fn test_empty_reader() {
    let data = [];
    let mut reader = CorrectLookAheadBitwiseReader::new(LookAheadBitwiseReader::new(&data[..]));

    assert_eq!(reader.consume_bits(0).unwrap(), vec![]);
    assert_eq!(reader.consume_bits(1).unwrap(), vec![false; 1]);
    assert_eq!(reader.consume_bits(2).unwrap(), vec![false; 2]);
    assert_eq!(reader.consume_bits(128).unwrap(), vec![false; 128]);

    assert_eq!(reader.look_ahead_bits(0).unwrap(), vec![]);
    assert_eq!(reader.look_ahead_bits(1).unwrap(), vec![false; 1]);
    assert_eq!(reader.look_ahead_bits(2).unwrap(), vec![false; 2]);
    assert_eq!(reader.look_ahead_bits(128).unwrap(), vec![false; 128]);
  }

  #[test]
  fn test_sample_bitreader() {
    let data = [0x30, 0x30, 0x03];
    let mut reader = CorrectLookAheadBitwiseReader::new(LookAheadBitwiseReader::new(&data[..]));

    assert_eq!(reader.consume_bits(0).unwrap(), vec![]);
    assert_eq!(reader.consume::<u16>(16).unwrap(), 0b0011_0000__0011_0000);
    assert_eq!(reader.consume::<u16>(16).unwrap(), 0b0000_0011__0011_0000);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b0000_0);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b011__00);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b11_000);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b0__0000);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b0011__0);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b011_00);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b00__000);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b0_0011);
    assert_eq!(reader.consume::<u16>(16).unwrap(), 0b0011_0000__0000_0011);
    assert_eq!(reader.look_ahead::<u8>(8).unwrap(), 0b0011_0000);
    assert_eq!(reader.consume::<u8>(7).unwrap(), 0b0011_000);
    assert_eq!(reader.look_ahead::<u8>(8).unwrap(), 0b0__0000_001);
  }
  #[test]
  fn test_eof() {
    let data = (0..=255u8).collect::<Vec<_>>();
    let mut reader = CorrectLookAheadBitwiseReader::new(LookAheadBitwiseReader::new(&data[..]));

    let mut counter = 0usize;
    while !reader.is_eof() {
      assert!(
        counter <= 256 * 8,
        "Counter: {:#X}; EOF: {:?}",
        counter,
        reader.eof_bits()
      );
      assert_eq!(
        reader.look_ahead::<bool>(1).unwrap(),
        reader.consume(1).unwrap()
      );
      counter += 1;
    }
    assert_eq!(counter, (256 * 8) + 1);
    assert_eq!(reader.eof_bits(), 1);
    assert_eq!(reader.look_ahead::<u16>(15).unwrap(), 0b111_1110__1111_1111);
  }
}
