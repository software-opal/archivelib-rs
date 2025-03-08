use std::convert::TryInto;
use std::fmt::Debug;

use crate::support::bit_iter::ToBits;

pub trait BitwiseWrite {
  fn write_bits<B, L>(&mut self, bits: B, bit_count: L) -> std::io::Result<usize>
  where
    B: TryInto<u128> + Debug + Copy,
    L: TryInto<usize> + Debug + Copy;
  fn finalise(&mut self) -> std::io::Result<()>;
}

pub struct BitwiseWriter<W: std::io::Write> {
  inner: W,
  buffer: Vec<bool>,
}

impl<W: std::io::Write> BitwiseWriter<W> {
  pub fn new(w: W) -> Self {
    Self {
      inner: w,
      buffer: Vec::with_capacity(8),
    }
  }
  pub fn commit_buffer(&mut self) -> std::io::Result<usize> {
    if self.buffer.len() >= 8 {
      let mut to_write = Vec::with_capacity(self.buffer.len() / 8);
      while self.buffer.len() >= 8 {
        // let this_byte = self.buffer.drain(..8);
        // let mut byte = 0;
        // for bit in this_byte {
        //   byte = (byte << 1) | (if bit { 1 } else { 0 })
        // }
        let byte = ((self.buffer[0] as u8) << 7)
          | ((self.buffer[1] as u8) << 6)
          | ((self.buffer[2] as u8) << 5)
          | ((self.buffer[3] as u8) << 4)
          | ((self.buffer[4] as u8) << 3)
          | ((self.buffer[5] as u8) << 2)
          | ((self.buffer[6] as u8) << 1)
          | (self.buffer[7] as u8);
        self.buffer.drain(..8);
        to_write.push(byte);
      }
      self.inner.write_all(&to_write)?;
    }
    Ok(self.buffer.len())
  }
}

impl<W: std::io::Write> BitwiseWrite for BitwiseWriter<W> {
  fn write_bits<B, L>(&mut self, bits: B, bit_count: L) -> std::io::Result<usize>
  where
    B: TryInto<u128> + Debug + Copy,
    L: TryInto<usize> + Debug + Copy,
  {
    let bits = bits
      .try_into()
      .map_err(|_| format!("Cannot convert bits({:#X?}) to u128", bits))
      .unwrap();
    let bit_count = bit_count
      .try_into()
      .map_err(|_| format!("Cannot convert bit_count({:#X?}) to usize", bits))
      .unwrap();
    if bit_count > 0 {
      let bit_array = bits.to_bits();
      self
        .buffer
        .extend(bit_array.iter().skip(bit_array.len() - bit_count));
    }
    self.commit_buffer()
  }
  fn finalise(&mut self) -> std::io::Result<()> {
    let unwritten = self.buffer.len() % 8;
    if unwritten > 0 {
      self.write_bits(0, 8 - unwritten)?;
    }
    Ok(())
  }
}

#[cfg(test)]
mod tests {
  use super::*;

  #[test]
  fn test_bool_to_u8() {
    assert_eq!(true as u8, 1);
  }

  #[test]
  fn test_write_bits() {
    let mut buf = Vec::new();
    let mut writer = BitwiseWriter::new(&mut buf);
    assert_eq!(writer.write_bits(0xF0E1, 8).unwrap(), 0);
    assert_eq!(buf, [0xE1]);
  }

  #[test]
  fn test_write_odd_number_of_bits() {
    let mut buf = Vec::new();
    let mut writer = BitwiseWriter::new(&mut buf);
    assert_eq!(writer.write_bits(0xF0E1D2C3_u32, 20).unwrap(), 4);
    assert_bytes_eq!(buf, [0x1D, 0x2C]);
  }
  #[test]
  fn test_finalise_pads_last_byte_with_zeros() {
    let mut buf = Vec::new();
    let mut writer = BitwiseWriter::new(&mut buf);
    assert_eq!(writer.write_bits(0xF_u32, 2).unwrap(), 2);
    writer.finalise().unwrap();
    assert_bytes_eq!(buf, [0b1100_0000]);
  }

  #[test]
  fn test_write_large_numbers_of_bits() {
    let mut buf = Vec::new();
    let mut writer = BitwiseWriter::new(&mut buf);
    assert_eq!(writer.write_bits(!0_u128, 128).unwrap(), 0);
    assert_bytes_eq!(buf, [0xFF; 16]);
  }
}
