use crate::support::bit_iter::{FromBits, ToBits};
use std::io;

fn pad_bit_arr(mut bit_data: Vec<bool>, target_len: usize) -> Vec<bool> {
  for _ in bit_data.len()..target_len {
    bit_data.push(false);
  }
  bit_data
}

pub trait LookAheadBitwiseRead {
  fn consume_bits(&mut self, bits: usize) -> io::Result<Vec<bool>>;
  fn look_ahead_bits(&mut self, bits: usize) -> io::Result<Vec<bool>>;

  fn consume<T>(&mut self, bits: usize) -> io::Result<T>
  where
    T: FromBits,
  {
    assert!(
      bits <= T::size(),
      "Requested bit size would be out of bounds"
    );
    Ok(T::from_bits(pad_bit_arr(self.consume_bits(bits)?, bits)))
  }
  fn look_ahead<T>(&mut self, bits: usize) -> io::Result<T>
  where
    T: FromBits,
  {
    assert!(
      bits <= T::size(),
      "Requested bit size would be out of bounds"
    );
    Ok(T::from_bits(pad_bit_arr(self.look_ahead_bits(bits)?, bits)))
  }
  fn look_ahead_skip<T>(&mut self, skip: usize, bits: usize) -> io::Result<T>
  where
    T: FromBits,
  {
    assert!(
      bits <= T::size(),
      "Requested bit size would be out of bounds"
    );
    let mut all_bits = self.look_ahead_bits(bits + skip)?;
    all_bits.drain(..skip);
    Ok(T::from_bits(pad_bit_arr(all_bits, bits)))
  }
}

pub struct LookAheadBitwiseReader<R: io::Read> {
  inner: R,
  buffer: Vec<bool>,
}

impl<R: io::Read> LookAheadBitwiseReader<R> {
  pub fn new(reader: R) -> Self {
    LookAheadBitwiseReader {
      inner: reader,
      buffer: vec![],
    }
  }
  pub fn ensure_buffer(&mut self, min_buffer_size: usize) -> io::Result<bool> {
    while self.buffer.len() < min_buffer_size {
      let bytes_to_read = 1 + (min_buffer_size - self.buffer.len()) / 8;
      self.buffer.reserve(bytes_to_read * 8);
      let mut block = vec![0u8; bytes_to_read];
      match self.inner.read(&mut block) {
        Err(e) => match e.kind() {
          io::ErrorKind::Interrupted => continue,
          _ => return Err(e),
        },
        Ok(0) => break,
        Ok(count) => {
          // println!("Read: {:X?}", &block[..count]);
          self
            .buffer
            .extend(block[..count].iter().flat_map(|&v| v.to_bits().into_vec()))
        }
      }
    }
    Ok(self.buffer.len() >= min_buffer_size)
  }
}

impl<R: io::Read> LookAheadBitwiseRead for LookAheadBitwiseReader<R> {
  fn consume_bits(&mut self, bits: usize) -> io::Result<Vec<bool>> {
    let range = if self.ensure_buffer(bits)? {
      ..bits
    } else {
      ..self.buffer.len()
    };
    let data = self.buffer.drain(range).collect();
    // println!("Consume {}: {:?}", bits, data);
    Ok(data)
  }
  fn look_ahead_bits(&mut self, bits: usize) -> io::Result<Vec<bool>> {
    if self.ensure_buffer(bits)? {
      // println!("Look ahead {}: {:X?}", bits, &self.buffer[..bits]);
      Ok(self.buffer[..bits].to_vec())
    } else {
      // println!("Look ahead {}: {:X?}", bits, &self.buffer[..]);
      Ok(self.buffer[..].to_vec())
    }
  }
}

pub struct ExpectedCallLookAheadBitwiseReader {
  data: Vec<bool>,
  index: usize,
  consume_calls: Vec<usize>,
  consume_call_index: usize,
}

impl ExpectedCallLookAheadBitwiseReader {
  pub fn new(data: impl ToBits, consume_calls: &[usize]) -> Self {
    ExpectedCallLookAheadBitwiseReader {
      data: data.to_bits().into_vec(),
      index: 0,
      consume_calls: consume_calls.to_vec(),
      consume_call_index: 0,
    }
  }
}

impl LookAheadBitwiseRead for ExpectedCallLookAheadBitwiseReader {
  fn consume_bits(&mut self, bits: usize) -> io::Result<Vec<bool>> {
    assert!(
      self.consume_call_index < self.consume_calls.len(),
      "Unexpected consume call for {} bits; Too many calls",
      bits
    );
    let expected = self.consume_calls[self.consume_call_index];
    assert_eq!(
      bits, expected,
      "Unexpected consume call(#{}) for {} bits; was expecting a call for {} bits",
      self.consume_call_index, bits, expected
    );

    let items = self.look_ahead_bits(bits)?;
    self.index += bits;
    self.consume_call_index += 1;
    Ok(items)
  }
  fn look_ahead_bits(&mut self, bits: usize) -> io::Result<Vec<bool>> {
    let data = self
      .data
      .iter()
      .skip(self.index)
      .take(bits)
      .cloned()
      .collect();
    Ok(data)
  }
}

#[cfg(test)]
mod tests {
  use super::*;

  #[test]
  fn reader_calls() {
    let data: Vec<u8> = vec![0x00, 0x03, 0x20, 0x04, 0x3F, 0xF0, 0x1A, 0xE7, 0xC0, 0x02];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);

    assert_eq!(reader.consume::<u16>(16).unwrap(), 0b0000000000000011);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b0000000000000100);
  }

  #[test]
  fn reader_matches_c_implementation_real_data() {
    let data: Vec<u8> = vec![0x30, 0x30, 0x03];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);

    for &(expected, advance) in &[
      (0x3030, 16),
      (0x0330, 16),
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
    ] {
      let actual = reader.look_ahead::<u16>(16).unwrap();
      assert_eq!(
        actual, expected,
        "Expected {:#X}, got {:#X}",
        expected, actual
      );
      reader.consume_bits(advance).unwrap();
    }
  }

  #[test]
  fn reader_matches_c_implementation_testing() {
    let data: Vec<u8> = vec![0b11001110, 0b00011010, 0b11001001];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);

    assert_eq!(
      reader.look_ahead_bits(16).unwrap(),
      binary_vec![1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0]
    )
  }

  #[test]
  #[should_panic(expected = "Requested bit size would be out of bounds")]
  fn reader_calls_big_call() {
    let data: Vec<u8> = vec![0xff];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);

    let _ = reader.consume::<u8>(420);
    assert!(false, "Test failed as consume did not panic")
  }
}
