use super::BitwiseIterable;
use std::io;

pub trait BitwiseWrite {
  fn write_bits(&mut self, bits: impl Into<u128>, bit_count: usize) -> io::Result<usize>;
}

pub struct BitwiseWriter<W: io::Write> {
  inner: W,
  buffer: Vec<bool>,
}

impl<W: io::Write> BitwiseWriter<W> {
  pub fn new(w: W) -> Self {
    BitwiseWriter {
      inner: w,
      buffer: Vec::with_capacity(8),
    }
  }
  pub fn checked_into_inner(self) -> W {
    assert_eq!(self.buffer, vec![]);
    return self.into_inner();
  }
  pub fn into_inner(self) -> W {
    return self.inner;
  }
  pub fn commit_buffer(&mut self) -> io::Result<usize> {
    if self.buffer.len() >= 8 {
      let mut to_write = Vec::with_capacity(self.buffer.len() / 8);
      while self.buffer.len() >= 8 {
        let this_byte = self.buffer.drain(..8);
        let mut byte = 0;
        for bit in this_byte {
          byte = (byte << 1) | (if bit { 1 } else { 0 })
        }
        to_write.push(byte);
      }
      self.inner.write_all(&to_write)?;
    }
    return Ok(self.buffer.len());
  }
}

impl<W: io::Write> BitwiseWrite for BitwiseWriter<W> {
  fn write_bits(&mut self, bits: impl Into<u128>, bit_count: usize) -> io::Result<usize> {
    if bit_count > 0 {
      // 'bit_array' starts out as LSB-MSB, but we want to reverse that order so we can add it to
      // the buffer array in MSB-LSB(the way we'll write it out)
      let mut bit_array = bits.into().into_bits()[..bit_count].to_vec();
      bit_array.reverse();
      self.buffer.extend(bit_array.into_iter());
    }
    self.commit_buffer()
  }
}
