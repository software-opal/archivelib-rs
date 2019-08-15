use super::base::LookAheadBitwiseRead;

pub trait CorrectLookAheadBitwiseRead: LookAheadBitwiseRead {
  fn is_eof(&self) -> bool;
}
pub struct CorrectLookAheadBitwiseReader<R: LookAheadBitwiseRead> {
  reader: R,
  is_eof: Option<usize>,
  buffer: [bool; 16],
}

impl<R: LookAheadBitwiseRead> CorrectLookAheadBitwiseReader<R> {
  fn new(reader: R) -> Self {
    CorrectLookAheadBitwiseReader {
      reader,
      is_eof: None,
      buffer: [false; 16],
    }
  }
}

impl<R: LookAheadBitwiseRead> From<R> for CorrectLookAheadBitwiseReader<R> {
  fn from(reader: R) -> CorrectLookAheadBitwiseReader<R> {
    Self::new(reader)
  }
}

impl<R: LookAheadBitwiseRead> CorrectLookAheadBitwiseRead for CorrectLookAheadBitwiseReader<R> {
  fn is_eof(&self) -> bool {
    self.is_eof == Some(0)
  }
}
impl<R: LookAheadBitwiseRead> LookAheadBitwiseRead for CorrectLookAheadBitwiseReader<R> {
  fn consume_bits(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    if bits == 0 {
      return Ok(vec![]);
    }
    let mut consumed = self.reader.consume_bits(bits)?;
    assert!(consumed.len() <= bits );
    if consumed.len() >= self.buffer.len() {
      let start = consumed.len() - self.buffer.len();
      self.buffer.copy_from_slice(&consumed[start..]);
    } else if consumed.len() > 0 {
      let start = self.buffer.len() - consumed.len();
      self.buffer.rotate_left(consumed.len());
      self.buffer[start..].copy_from_slice(&consumed[..]);
    }
    consumed.reserve(bits - consumed.len());
    while consumed.len() < bits {
      let to_move = bits - consumed.len();
      if to_move >= self.buffer.len() {
        // Move at most 1 buffer load at a time to make life easy.
        consumed.copy_from_slice(&self.buffer);
      } else {
        consumed.copy_from_slice(&self.buffer[..to_move]);
        self.buffer.rotate_left(to_move);
      }
    }
    assert_eq!(consumed.len() , bits);
    Ok(consumed)
  }
  fn look_ahead_bits(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    Ok(self.reader.look_ahead_bits(bits)?)
  }
}

#[cfg(test)]
mod tests {
  use super::*;
  use crate::support::{
    ExpectedCallLookAheadBitwiseReader, LookAheadBitwiseRead, LookAheadBitwiseReader,
  };

  #[test]
  fn test_empty_reader() {
    let data = [];
    let mut reader = CorrectLookAheadBitwiseReader::new(LookAheadBitwiseReader::new(&data[..]));

    assert_eq!(reader.consume_bits(0).unwrap(), vec![]);
    assert_eq!(reader.consume_bits(1).unwrap(), vec![false; 1]);
    assert_eq!(reader.consume_bits(2).unwrap(), vec![false; 2]);
    assert_eq!(reader.consume_bits(128).unwrap(), vec![false; 128]);
  }
}
