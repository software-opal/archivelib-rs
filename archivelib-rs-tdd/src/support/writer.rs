use super::BitwiseIterable;
use num::ToPrimitive;
use std::io;

pub trait BitwiseWrite {
  fn write_bits(
    &mut self,
    bits: impl ToPrimitive,
    bit_count: impl ToPrimitive,
  ) -> io::Result<usize>;
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
  fn write_bits(
    &mut self,
    bits_: impl ToPrimitive,
    bit_count_: impl ToPrimitive,
  ) -> io::Result<usize> {
    let bits = bits_.to_u128().unwrap();
    let bit_count = bit_count_.to_usize().unwrap();
    if bit_count > 0 {
      // 'bit_array' starts out as LSB-MSB, but we want to reverse that order so we can add it to
      // the buffer array in MSB-LSB(the way we'll write it out)
      let mut bit_array = bits.into_bits()[..bit_count].to_vec();
      bit_array.reverse();
      self.buffer.extend(bit_array.into_iter());
    }
    self.commit_buffer()
  }
}

pub struct ExactCallWriter {
  calls: Vec<(u128, usize)>,
  write_calls: usize,
  pub written_bits: usize,
}

impl ExactCallWriter {
  pub fn from_vec(calls: Vec<(u128, usize)>) -> Self {
    return Self {
      calls: calls,
      write_calls: 0,
      written_bits: 0,
    };
  }
  pub fn assert_drained(&self) {
    assert_eq!(self.calls, vec![])
  }
}
impl BitwiseWrite for ExactCallWriter {
  fn write_bits(
    &mut self,
    bits_: impl ToPrimitive,
    bit_count_: impl ToPrimitive,
  ) -> io::Result<usize> {
    let bits = bits_.to_u128().unwrap();
    let bit_count = bit_count_.to_usize().unwrap();
    assert!(
      !self.calls.is_empty(),
      "Attempting to call write_bits({:X}, {}) when it wasn't expected; Calls expended",
      bits,
      bit_count
    );
    let actual = (bits, bit_count);
    let expected = self.calls.remove(0);
    assert_eq!(
      actual, expected,
      "Trying to write {:?} when {:?} was expected at index {}",
      actual, expected, self.write_calls
    );
    self.written_bits += bit_count;
    self.write_calls += 1;
    Ok(self.written_bits % 8)
  }
}

pub struct NullBitwiseWriter {}
impl NullBitwiseWriter {
  pub fn new() -> Self {
    NullBitwiseWriter {}
  }
}
impl BitwiseWrite for NullBitwiseWriter {
  fn write_bits(&mut self, _: impl ToPrimitive, _: impl ToPrimitive) -> io::Result<usize> {
    return Ok(0);
  }
}
