use crate::support::bit_iter::{FromBits, ToBits};

fn pad_bit_arr(mut bit_data: Vec<bool>, target_len: usize) -> Vec<bool> {
  for _ in bit_data.len()..target_len {
    bit_data.push(false);
  }
  bit_data
}

pub trait LookAheadBitwiseRead {
  fn consume_bits(&mut self, bits: usize) -> std::io::Result<Vec<bool>>;
  fn look_ahead_bits(&mut self, bits: usize) -> std::io::Result<Vec<bool>>;

  fn consume<T>(&mut self, bits: usize) -> std::io::Result<T>
  where
    T: FromBits,
  {
    assert!(
      bits <= T::size(),
      "Requested bit size would be out of bounds. Requested {}; max size: {}",
      bits,
      T::size()
    );
    Ok(T::from_bits(pad_bit_arr(self.consume_bits(bits)?, bits)))
  }
  fn look_ahead<T>(&mut self, bits: usize) -> std::io::Result<T>
  where
    T: FromBits,
  {
    assert!(
      bits <= T::size(),
      "Requested bit size would be out of bounds. Requested {}; max size: {}",
      bits,
      T::size()
    );
    Ok(T::from_bits(pad_bit_arr(self.look_ahead_bits(bits)?, bits)))
  }
  fn look_ahead_skip<T>(&mut self, skip: usize, bits: usize) -> std::io::Result<T>
  where
    T: FromBits,
  {
    assert!(
      bits <= T::size(),
      "Requested bit size would be out of bounds. Requested {}; max size: {}",
      bits,
      T::size()
    );
    let mut all_bits = self.look_ahead_bits(bits + skip)?;
    all_bits.drain(..skip);
    Ok(T::from_bits(pad_bit_arr(all_bits, bits)))
  }
}

pub struct LookAheadBitwiseReader<R: std::io::Read> {
  inner: R,
  buffer: Vec<bool>,
}

impl<R: std::io::Read> LookAheadBitwiseReader<R> {
  pub fn new(reader: R) -> Self {
    Self {
      inner: reader,
      buffer: vec![],
    }
  }
  pub fn ensure_buffer(&mut self, min_buffer_size: usize) -> std::io::Result<bool> {
    while self.buffer.len() < min_buffer_size {
      let bytes_to_read = 1 + (min_buffer_size - self.buffer.len()) / 8;
      self.buffer.reserve(bytes_to_read * 8);
      let mut block = vec![0_u8; bytes_to_read];
      match self.inner.read(&mut block) {
        Err(e) => match e.kind() {
          std::io::ErrorKind::Interrupted => continue,
          _ => return Err(e),
        },
        Ok(0) => break,
        Ok(count) => self
          .buffer
          .extend(block[..count].iter().flat_map(|&v| v.to_bits().into_vec())),
      }
    }
    Ok(self.buffer.len() >= min_buffer_size)
  }
}

impl<R: std::io::Read> LookAheadBitwiseRead for LookAheadBitwiseReader<R> {
  fn consume_bits(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    let range = if self.ensure_buffer(bits)? {
      ..bits
    } else {
      ..self.buffer.len()
    };
    let data = self.buffer.drain(range).collect();
    Ok(data)
  }
  fn look_ahead_bits(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    if self.ensure_buffer(bits)? {
      Ok(self.buffer[..bits].to_vec())
    } else {
      Ok(self.buffer[..].to_vec())
    }
  }
}

#[cfg(test)]
mod tests {
  use super::*;

  #[test]
  fn reader_calls() {
    let data: Vec<u8> = vec![0x00, 0x03, 0x20, 0x04, 0x3F, 0xF0, 0x1A, 0xE7, 0xC0, 0x02];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);

    assert_eq!(reader.consume::<u16>(16).unwrap(), 0b0000_0000_0000_0011);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b0000_0000_0000_0100);
  }

  #[test]
  fn reader_matches_c_implementation_testing() {
    let data: Vec<u8> = vec![0b1100_1110, 0b0001_1010, 0b1100_1001];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);

    assert_eq!(reader.look_ahead::<u16>(16).unwrap(), 0b1100_1110_0001_1010)
  }

  #[test]
  #[should_panic(expected = "Requested bit size would be out of bounds")]
  fn reader_calls_big_call() {
    let data: Vec<u8> = vec![0xff];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);

    let _ = reader.consume::<u8>(420);
    panic!("Test failed as consume did not panic")
  }
}
