#![allow(clippy::module_name_repetitions)]

use super::base::{LookAheadBitwiseRead, LookAheadBitwiseReader};
use crate::consts::BUFFER_BIT_SIZE;
use crate::consts::EOF_ERROR_LIMIT;

pub trait CorrectLookAheadBitwiseRead: LookAheadBitwiseRead {
  fn al_eof_error_count(&self) -> usize;
  fn is_al_eof(&self) -> bool;
  fn consume_bits_nopad(&mut self, bits: usize) -> std::io::Result<Vec<bool>>;
  fn look_ahead_bits_nopad(&mut self, bits: usize) -> std::io::Result<Vec<bool>>;
}
pub struct CorrectLookAheadBitwiseReader<R: LookAheadBitwiseRead> {
  reader: R,
  read_bits: usize,
  file_start: Vec<bool>,
  eof_calls: Vec<usize>,
}

impl<R: LookAheadBitwiseRead> CorrectLookAheadBitwiseReader<R> {
  pub fn new(reader: R) -> Self {
    Self {
      reader,
      read_bits: 0,
      file_start: Vec::with_capacity(8),
      eof_calls: Vec::with_capacity(6),
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
    if self.file_start.is_empty() {
      buffer.resize(bits, false);
      0
    } else {
      assert_eq!(self.file_start.len(), 8);
      while buffer.len() + self.file_start.len() <= bits {
        buffer.extend_from_slice(&self.file_start);
      }
      assert!(bits - buffer.len() < self.file_start.len());
      if buffer.len() < bits {
        let remaining = bits - buffer.len();
        buffer.extend_from_slice(&self.file_start[..remaining]);
        remaining
      } else {
        assert_eq!(buffer.len(), bits);
        0
      }
    }
  }
}

impl<I: std::io::Read> CorrectLookAheadBitwiseReader<LookAheadBitwiseReader<I>> {
  pub fn from_reader(reader: I) -> Self {
    Self::new(LookAheadBitwiseReader::new(reader))
  }
}

impl<R: LookAheadBitwiseRead> From<R> for CorrectLookAheadBitwiseReader<R> {
  fn from(reader: R) -> Self {
    Self::new(reader)
  }
}

impl<R: LookAheadBitwiseRead> CorrectLookAheadBitwiseRead for CorrectLookAheadBitwiseReader<R> {
  fn al_eof_error_count(&self) -> usize {
    if self.eof_calls.is_empty() {
      return 0;
    }
    let count = self.eof_calls.iter().fold(0, |a, b| a + b);
    if count == 0 {
      return 2;
    } else {
      return 2 + ((count + 7) / 8);
    }
  }
  fn is_al_eof(&self) -> bool {
    self.al_eof_error_count() < EOF_ERROR_LIMIT
  }

  fn consume_bits_nopad(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    if bits == 0 {
      return Ok(vec![]);
    }
    let consumed = self.reader.consume_bits(bits)?;
    if consumed.is_empty() {
      Ok(consumed)
    } else {
      // The C library uses the first byte of the *buffer* once it runs out.
      // I don't think it is actually used in normal operation; but I want to
      // replicate the implementation(warts and all)
      assert!(self.read_bits <= BUFFER_BIT_SIZE, "{}", self.read_bits);
      let start;
      if (self.read_bits + consumed.len()) > BUFFER_BIT_SIZE {
        start = BUFFER_BIT_SIZE - self.read_bits;
        self.read_bits = consumed.len() - start;
        self.file_start.clear();
      } else {
        self.read_bits += consumed.len();
        start = 0;
      }
      if self.file_start.len() < 8 {
        let size = usize::min(8 - self.file_start.len(), consumed.len() - start);
        let end = start + size;
        self.file_start.extend_from_slice(&consumed[start..end]);
      }
      assert!(self.file_start.len() <= 8, "{}", self.file_start.len());
      assert!(self.read_bits <= BUFFER_BIT_SIZE, "{}", self.read_bits);
      Ok(consumed)
    }
  }
  fn look_ahead_bits_nopad(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    self.reader.look_ahead_bits(bits)
  }
}
impl<R: LookAheadBitwiseRead> LookAheadBitwiseRead for CorrectLookAheadBitwiseReader<R> {
  fn consume_bits(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    if bits == 0 {
      return Ok(vec![]);
    }
    let mut consumed = self.consume_bits_nopad(bits)?;
    assert!(consumed.len() <= bits);
    if bits != consumed.len() {
      self.eof_calls.push(bits - consumed.len());
      let to_shift = self.pad_buffer(bits, &mut consumed);
      self.file_start.rotate_left(to_shift);
    } else if self.look_ahead_bits_nopad(1)?.is_empty() {
      self.eof_calls.push(0);
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
  use crate::consts::{BUFFER_BIT_SIZE, BUFFER_SIZE};
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
  fn test_real_example() {
    let data = [0x30, 0x30, 0x00];
    let mut reader = CorrectLookAheadBitwiseReader::new(LookAheadBitwiseReader::new(&data[..]));
    assert_eq!(
      reader.consume_bits(16).unwrap(),
      vec![
        false, false, true, true, false, false, false, false, false, false, true, true, false,
        false, false, false,
      ],
      "Expected: {:b}",
      12336
    );
    assert_eq!(
      reader.consume_bits(5).unwrap(),
      vec![false; 5],
      "Expected: {:b}",
      0
    );
    assert_eq!(
      reader.consume_bits(5).unwrap(),
      vec![false; 5],
      "Expected: {:b}",
      0
    );
    assert_eq!(reader.al_eof_error_count(), 3);
    assert_eq!(
      reader.file_start,
      vec![true, true, false, false, false, false, false, false]
    );
    assert_eq!(
      reader.consume_bits(9).unwrap(),
      vec![true, true, false, false, false, false, false, false, true,],
      "Expected: {:b}",
      385
    );
  }

  #[test]
  fn test_buffer_size() {
    let mut data = vec![0xff; BUFFER_SIZE];
    data.resize(BUFFER_SIZE * 2, 0x00);
    data.push(0x96);
    data.push(0x55);
    let mut reader = CorrectLookAheadBitwiseReader::new(LookAheadBitwiseReader::new(&data[..]));

    assert_eq!(reader.file_start, vec![]);
    assert_eq!(reader.consume_bits_nopad(8).unwrap(), vec![true; 8]);
    assert_eq!(reader.file_start, vec![true; 8]);
    assert_eq!(reader.consume_bits_nopad(8).unwrap(), vec![true; 8]);
    assert_eq!(reader.file_start, vec![true; 8]);
    for i in 2..BUFFER_SIZE {
      assert_eq!(
        reader.consume_bits_nopad(8).unwrap(),
        vec![true; 8],
        "Byte {}",
        i
      );
      assert_eq!(reader.file_start, vec![true; 8]);
      assert_eq!(reader.read_bits, (i + 1) * 8);
    }
    // Buffer is reset in this call
    assert_eq!(reader.consume_bits_nopad(1).unwrap(), vec![false]);
    assert_eq!(reader.file_start, vec![false]);
    assert_eq!(reader.consume_bits_nopad(5).unwrap(), vec![false; 5]);
    assert_eq!(reader.file_start, vec![false; 6]);
    assert_eq!(reader.consume_bits_nopad(5).unwrap(), vec![false; 5]);
    assert_eq!(reader.file_start, vec![false; 8]);
    assert_eq!(reader.consume_bits_nopad(5).unwrap(), vec![false; 5]);
    assert_eq!(reader.file_start, vec![false; 8]);
    for i in 2..(BUFFER_SIZE - 1) {
      assert_eq!(
        reader.consume_bits_nopad(8).unwrap(),
        vec![false; 8],
        "Byte {}",
        i
      );
      assert_eq!(reader.file_start, vec![false; 8]);
      assert_eq!(reader.read_bits, (i + 1) * 8);
    }
    assert_eq!(reader.read_bits, BUFFER_BIT_SIZE - 8);
    // 1 byte short of buffer reset
    // Buffer is reset in this call
    assert_eq!(
      reader.consume_bits_nopad(10).unwrap(),
      vec![
        false, false, false, false, false, false, false, false, // Buffer resets here
        true, false
      ]
    );
    assert_eq!(reader.read_bits, 2);
    assert_eq!(reader.file_start, vec![true, false]);
    assert_eq!(
      reader.consume_bits_nopad(3).unwrap(),
      vec![false, true, false]
    );
    assert_eq!(reader.file_start, vec![true, false, false, true, false]);
    assert_eq!(reader.consume_bits_nopad(1).unwrap(), vec![true]);
    assert_eq!(
      reader.file_start,
      vec![true, false, false, true, false, true]
    );
    assert_eq!(
      reader.consume_bits_nopad(8).unwrap(),
      vec![
        true, false, // End of 0x96 / Start of 0x55
        false, true, false, true, false, true
      ]
    );
    assert_eq!(
      reader.file_start,
      vec![true, false, false, true, false, true, true, false]
    );
    assert_eq!(
      reader.consume_bits(4).unwrap(),
      vec![false, true, true, false]
    );
    assert_eq!(reader.read_bits, 16);
    assert_eq!(reader.al_eof_error_count(), 3);
    assert_eq!(
      reader.file_start,
      // Rotated 2 bits off the front
      vec![false, true, false, true, true, false, true, false]
    );
    assert_eq!(
      reader.consume_bits(4).unwrap(),
      vec![false, true, false, true]
    );
    assert_eq!(reader.al_eof_error_count(), 3);
    assert_eq!(
      reader.file_start,
      // Rotated 4 bits off the front
      vec![true, false, true, false, false, true, false, true]
    );
  }

  #[test]
  fn test_sample_bitreader() {
    let data = [0x30, 0x30, 0x03];
    let mut reader = CorrectLookAheadBitwiseReader::new(LookAheadBitwiseReader::new(&data[..]));

    assert_eq!(reader.consume_bits(0).unwrap(), vec![]);
    assert_eq!(reader.consume::<u16>(16).unwrap(), 0b0011_0000_0011_0000);
    assert_eq!(reader.consume::<u16>(16).unwrap(), 0b0000_0011_0011_0000);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b0011_0);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b000__00);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b11_000);
  }

  #[test]
  fn test_eof() {
    let mut data = (0..=255_u8).collect::<Vec<_>>();
    data.reverse();
    let mut reader = CorrectLookAheadBitwiseReader::new(LookAheadBitwiseReader::new(&data[..]));

    let mut counter = 0_usize;
    while reader.al_eof_error_count() <= 2 {
      assert!(
        counter <= 256 * 8,
        "Counter: {:#X}; EOF: {:?}",
        counter,
        reader.al_eof_error_count()
      );
      assert_eq!(
        reader.look_ahead::<bool>(1).unwrap(),
        reader.consume(1).unwrap()
      );
      if counter < (256 * 8) - 1 {
        assert_eq!(reader.al_eof_error_count(), 0);
      } else if counter == (256 * 8) - 1 {
        assert_eq!(reader.al_eof_error_count(), 2);
      } else {
        assert_eq!(reader.al_eof_error_count(), 3);
      }
      counter += 1;
    }
    assert_eq!(counter, (256 * 8) + 1);
    assert_eq!(reader.al_eof_error_count(), 3);
    assert_eq!(reader.look_ahead::<u8>(7).unwrap(), 0b111_1111);
  }

  #[test]
  fn test_eof_semantics_with_short_file_00_03() {
    let data = [0x00, 0x03];
    let mut reader = CorrectLookAheadBitwiseReader::from_reader(&data[..]);
    assert_eq!(
      reader.consume_bits(16).unwrap(),
      vec![
        false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, true, true
      ]
    );
    assert_eq!(reader.al_eof_error_count(), 2);
    assert_eq!(reader.consume_bits(5).unwrap(), vec![false; 5]);
    assert_eq!(reader.al_eof_error_count(), 3);
    assert_eq!(reader.consume_bits(5).unwrap(), vec![false; 5]);
    assert_eq!(reader.al_eof_error_count(), 4);
    assert_eq!(reader.consume_bits(9).unwrap(), vec![false; 9]);
    assert_eq!(reader.al_eof_error_count(), 5);
    assert_eq!(reader.consume_bits(9).unwrap(), vec![false; 9]);
    assert_eq!(reader.al_eof_error_count(), 6);
    assert_eq!(reader.consume_bits(5).unwrap(), vec![false; 5]);
    assert_eq!(reader.al_eof_error_count(), 7);
  }

  #[test]
  fn test_eof_semantics_with_short_file_05() {
    let data = [0x05];
    let mut reader = CorrectLookAheadBitwiseReader::from_reader(&data[..]);
    assert_eq!(
      reader.consume_bits(16).unwrap(),
      vec![
        false, false, false, false, false, true, false, true, false, false, false, false, false,
        true, false, true,
      ]
    );
    assert_eq!(reader.al_eof_error_count(), 3,);
    assert_eq!(
      reader.consume_bits(5).unwrap(),
      vec![false, false, false, false, false]
    );
    assert_eq!(reader.al_eof_error_count(), 4,);
    assert_eq!(
      reader.consume_bits(5).unwrap(),
      vec![true, false, true, false, false]
    );
    assert_eq!(reader.al_eof_error_count(), 5,);
    assert_eq!(
      reader.consume_bits(9).unwrap(),
      vec![false, false, false, true, false, true, false, false, false]
    );
    assert_eq!(reader.al_eof_error_count(), 6,);
    assert_eq!(
      reader.consume_bits(5).unwrap(),
      vec![false, false, true, false, true]
    );
    assert_eq!(reader.al_eof_error_count(), 6,);
    assert_eq!(reader.consume_bits(3).unwrap(), vec![false, false, false]);
    assert_eq!(reader.al_eof_error_count(), 7,);
    assert_eq!(reader.consume_bits(3).unwrap(), vec![false, false, true]);
    assert_eq!(reader.al_eof_error_count(), 7,);
    assert_eq!(reader.consume_bits(3).unwrap(), vec![false, true, false]);
    assert_eq!(reader.al_eof_error_count(), 8,);
    assert_eq!(reader.consume_bits(3).unwrap(), vec![false, false, false]);
    assert_eq!(reader.al_eof_error_count(), 8,);
    assert_eq!(reader.consume_bits(3).unwrap(), vec![false, true, false]);
    assert_eq!(reader.al_eof_error_count(), 8,);
  }
}
