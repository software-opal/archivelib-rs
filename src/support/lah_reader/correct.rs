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
    println!("Read: {:X?}; buffer: {:X?}", consumed, self.buffer);
    assert!(consumed.len() <= bits);
    if consumed.len() >= self.buffer.len() {
      let start = consumed.len() - self.buffer.len();
      self.buffer.copy_from_slice(&consumed[start..]);
    } else if consumed.len() > 0 {
      let start = self.buffer.len() - consumed.len();
      self.buffer.rotate_left(consumed.len());
      self.buffer[start..].copy_from_slice(&consumed[..]);
    }
    println!("Buffer updated: {:X?}; {:X?}", consumed, self.buffer);
    consumed.reserve(bits - consumed.len());
    while consumed.len() < bits {
      let to_move = bits - consumed.len();
      println!("To move remaining: {}", to_move);
      if to_move >= self.buffer.len() {
        // Move at most 1 buffer load at a time to make life easy.
        consumed.extend_from_slice(&self.buffer);
      } else {
        consumed.extend_from_slice(&self.buffer[..to_move]);
        self.buffer.rotate_left(to_move);
      }
    }
    println!("Consume padded: {:X?}; {:X?}", consumed, self.buffer);
    assert_eq!(consumed.len(), bits);
    Ok(consumed)
  }
  fn look_ahead_bits(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    if bits == 0 {
      return Ok(vec![]);
    }
    let mut lookahead = self.reader.look_ahead_bits(bits)?;

    while lookahead.len() + self.buffer.len() <= bits {
      lookahead.extend_from_slice(&self.buffer);
    }
    assert!(bits - lookahead.len() < self.buffer.len());
    if lookahead.len() < bits {
      let remaining = bits - lookahead.len();
      lookahead.extend_from_slice(&self.buffer[..remaining]);
    }
    assert_eq!(lookahead.len(), bits);

    Ok(lookahead)
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
    assert_eq!(reader.consume::<u16>(16).unwrap(), 0x30_30);
    assert_eq!(reader.consume::<u16>(16).unwrap(), 0x03_30);
    let expected = vec![
      (0x6606, 5),
      (0xc0c0, 5),
      (0x8181, 9),
      (0x8181, 0),
      (0x8181, 0),
      (0x8181, 3),
      (0x0c0c, 3),
      (0x6060, 3),
      (0x0303, 3),
      (0x1818, 3),
      (0xc0c0, 3),
      (0x0606, 3),
      (0x3030, 3),
      (0x8181, 3),
    ];
    for (i, (target, advance)) in expected.into_iter().enumerate() {
      reader.consume_bits(advance).unwrap();
      let actual = reader.look_ahead::<u16>(16).unwrap();
      assert_eq!(actual, target, "({})Expected {:#X}, got {:#X}", i, target, actual);
    }
  }
}
