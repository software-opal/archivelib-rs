use crate::support::{get_bitmask, BitwiseRead, ReadError};

type Result<T> = std::result::Result<T, ReadError>;

pub trait BitwiseReadAheadRead: BitwiseRead {
  fn read_ahead(&mut self, bits: usize) -> Result<u128>;
  fn consume(&mut self, bits: usize) -> Result<u128>;
}

pub struct BitwiseReadAheadReader<R: BitwiseRead> {
  inner: R,
  bit_buffer: u128,
  bit_buffer_len: usize,
  bit_buffer_pos: usize,
}

impl<R: BitwiseRead> BitwiseReadAheadReader<R> {
  pub fn new(reader: R) -> Self {
    BitwiseReadAheadReader {
      inner: reader,
      bit_buffer: 0,
      bit_buffer_len: 0,
      bit_buffer_pos: 0,
    }
  }
}

impl<R: BitwiseRead> BitwiseReadAheadRead for BitwiseReadAheadReader<R> {
  fn read_ahead(&mut self, bits: usize) -> Result<u128> {
    if bits == 0 {
      return Ok(0);
    }
    assert!(self.bit_buffer_pos + bits <= 128);
    let bitmask = get_bitmask(bits);

    if self.bit_buffer_pos + bits > self.bit_buffer_len {
      let to_read = (self.bit_buffer_pos + bits) - self.bit_buffer_len;
      let new_bytes = self.inner.read_bits(to_read)?;
      self.bit_buffer = (self.bit_buffer << to_read) | new_bytes;
      self.bit_buffer_len += to_read;
    }
    assert!(self.bit_buffer_pos + bits <= self.bit_buffer_len);

    let bits_rhs = self.bit_buffer_len - self.bit_buffer_pos - bits;
    let out = (self.bit_buffer >> bits_rhs) & bitmask;
    self.bit_buffer_pos += bits;
    return Ok(out);
  }
  fn consume(&mut self, bits: usize) -> Result<u128> {
    if bits > self.bit_buffer_len {
      let bits_to_load = bits - self.bit_buffer_len;
      self.read_ahead(bits_to_load)?;
    }
    self.bit_buffer_pos = 0;

    let rem_bits = self.bit_buffer_len - bits;
    let out = (self.bit_buffer >> (self.bit_buffer_len - bits)) & get_bitmask(bits);

    self.bit_buffer = self.bit_buffer & get_bitmask(rem_bits);
    self.bit_buffer_len = rem_bits;

    return Ok(out);
  }
}
impl<R: BitwiseRead> BitwiseRead for BitwiseReadAheadReader<R> {
  fn try_read_bits(&mut self, bits: usize) -> Result<(u8, usize)> {
    assert!(0 < bits && bits <= 8);
    let res = self.consume(bits)?;
    return Ok((res as u8, bits));
  }
}

#[cfg(test)]
mod tests {
  use super::*;
  use crate::support::{BitwiseReader, ReadError, VecReader};

  #[test]
  fn test_buffer() {
    let data = vec![0xFA, 0xCE];
    let mut reader = BitwiseReadAheadReader::new(BitwiseReader::new(VecReader::new(data)));

    assert_eq!(reader.read_ahead(4), Ok(0xF));
    assert_eq!(reader.read_ahead(4), Ok(0xA));
    assert_eq!(reader.consume(2), Ok(0x3));
    assert_eq!(reader.read_ahead(6), Ok(0x3A));
    assert_eq!(reader.consume(2), Ok(0x3));
    assert_eq!(reader.read_ahead(12), Ok(0xACE));
    assert_eq!(reader.consume(4), Ok(0xA));
    assert_eq!(reader.consume(4), Ok(0xC));
    assert_eq!(reader.consume(3), Ok(0x7));
    assert_eq!(reader.consume(1), Ok(0x0));
    assert_eq!(reader.consume(1), Err(ReadError::EndOfFile()));
  }
}
