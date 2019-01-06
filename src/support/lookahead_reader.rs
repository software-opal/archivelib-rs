use super::bit_iter::{FromBits, IntoBits};
use std::io;

pub trait LookAheadBitwiseRead {
  fn consume_bits(&mut self, bits: usize) -> io::Result<Vec<bool>>;
  fn look_ahead_bits(&mut self, bits: usize) -> io::Result<Vec<bool>>;

  fn consume<T>(&mut self, bits: usize) -> io::Result<T>
  where
    T: FromBits,
  {
    assert!(bits <= T::size());
    Ok(T::from_bits(self.consume_bits(bits)?))
  }
  fn look_ahead<T>(&mut self, bits: usize) -> io::Result<T>
  where
    T: FromBits,
  {
    assert!(bits <= T::size());
    Ok(T::from_bits(self.look_ahead_bits(bits)?))
  }
  fn look_ahead_skip<T>(&mut self, skip: usize, bits: usize) -> io::Result<T>
  where
    T: FromBits,
  {
    assert!(bits <= T::size());
    let mut all_bits = self.look_ahead_bits(bits + skip)?;
    all_bits.drain(..skip);
    Ok(T::from_bits(all_bits))
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
        Ok(count) => self.buffer.extend(
          block[..count]
            .iter()
            .flat_map(|&v| v.into_bits().into_vec()),
        ),
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
    println!("consume {}: {:X?}", bits, data);
    return Ok(data);
  }
  fn look_ahead_bits(&mut self, bits: usize) -> io::Result<Vec<bool>> {
    if self.ensure_buffer(bits)? {
      return Ok(self.buffer[..bits].to_vec());
    } else {
      return Ok(self.buffer[..].to_vec());
    }
  }
}
