use std::convert::TryInto;
use std::fmt::Debug;

use crate::support::bit_iter::ToBits;

pub trait BitwiseWrite {
  fn write_bits<B, L>(&mut self, bits: B, bit_count: L) -> std::io::Result<()>
  where
    B: TryInto<u16> + Debug + Copy,
    L: TryInto<u8> + Debug + Copy;
  fn finalise(&mut self) -> std::io::Result<()>;

  /// Largest number of bits that can be written in a single operation.
  ///
  /// Must match the size of `write_bits`'s `bits` argument.
  fn max_bit_count(&self) -> u8 {
    u16::BITS as u8
  }
}

pub struct BitwiseWriter<W: std::io::Write> {
  inner: W,
  buffer: Vec<bool>,
}

impl<W: std::io::Write> BitwiseWriter<W> {
  pub fn new(w: W) -> Self {
    Self {
      inner: w,
      buffer: Vec::with_capacity(512),
    }
  }
  pub fn flush_buffer(&mut self) -> std::io::Result<()> {
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
    Ok(())
  }
}

impl<W: std::io::Write> BitwiseWrite for BitwiseWriter<W> {
  fn write_bits<B, L>(&mut self, bits: B, bit_count: L) -> std::io::Result<()>
  where
    B: TryInto<u16> + Debug + Copy,
    L: TryInto<u8> + Debug + Copy,
  {
    let bits = bits
      .try_into()
      .map_err(|_| format!("Cannot convert bits({:#X?}) to u16", bits))
      .unwrap();
    let bit_count = bit_count
      .try_into()
      .map_err(|_| format!("Cannot convert bit_count({:#X?}) to u8", bits))
      .unwrap();
    assert!(bit_count <= 16);
    if bit_count > 0 {
      let bit_array = bits.to_bits();
      self.buffer.extend(
        bit_array
          .iter()
          .skip(bit_array.len() - (bit_count as usize)),
      );
    }
    if self.buffer.len() > 512 {
      self.flush_buffer()?;
    }
    Ok(())
  }
  fn finalise(&mut self) -> std::io::Result<()> {
    if self.buffer.len() == 0 {
      return Ok(());
    }
    let unwritten = self.buffer.len() % 8;
    if unwritten > 0 {
      self.write_bits(0, 8 - unwritten)?;
    }
    self.flush_buffer()
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
    writer.write_bits(0xF0E1, 8).unwrap();
    writer.flush_buffer().unwrap();
    assert_bytes_eq!([0xE1], buf);
  }

  #[test]
  fn test_write_odd_number_of_bits() {
    let mut buf = Vec::new();
    let mut writer = BitwiseWriter::new(&mut buf);
    writer.write_bits(0xD2C3_u16, 12).unwrap();
    writer.flush_buffer().unwrap();
    assert_bytes_eq!([0x2C], buf);
  }
  #[test]
  fn test_finalise_pads_last_byte_with_zeros() {
    let mut buf = Vec::new();
    let mut writer = BitwiseWriter::new(&mut buf);
    writer.write_bits(0xF_u32, 2).unwrap();
    writer.finalise().unwrap();
    assert_bytes_eq!([0b1100_0000], buf);
  }

  #[test]
  fn test_write_large_numbers_of_bits() {
    let mut buf = Vec::new();
    let mut writer = BitwiseWriter::new(&mut buf);
    writer.write_bits(!0_u16, 16).unwrap();
    writer.flush_buffer().unwrap();
    assert_bytes_eq!([0xFF; 2], buf);
  }

  #[test]
  fn test_flushes_buffer_after_a_large_number_of_writes() {
    let mut buf = Vec::new();
    let mut writer = BitwiseWriter::new(&mut buf);
    for _ in 0..1024 {
      writer.write_bits(0xFF, 8).unwrap();
    }
    assert!(buf.len() > 0);
  }
  #[test]
  fn test_doesnt_flush_after_a_single_bit_write() {
    let mut buf = Vec::new();
    let mut writer = BitwiseWriter::new(&mut buf);
    writer.write_bits(0xFF, 8).unwrap();
    assert_bytes_eq!([], buf);
  }
}
