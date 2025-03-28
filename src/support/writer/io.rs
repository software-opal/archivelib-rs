use crate::support::bit_utils::to_bits;

use super::BitwiseWrite;

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
        let byte = (u8::from(self.buffer[0]) << 7)
          | (u8::from(self.buffer[1]) << 6)
          | (u8::from(self.buffer[2]) << 5)
          | (u8::from(self.buffer[3]) << 4)
          | (u8::from(self.buffer[4]) << 3)
          | (u8::from(self.buffer[5]) << 2)
          | (u8::from(self.buffer[6]) << 1)
          | u8::from(self.buffer[7]);
        self.buffer.drain(..8);
        to_write.push(byte);
      }
      self.inner.write_all(&to_write)?;
    }
    Ok(())
  }
}

impl<W: std::io::Write> BitwiseWrite for BitwiseWriter<W> {
  fn write_bits(&mut self, bits: u16, bit_count: usize) -> std::io::Result<()> {
    assert!(
      bit_count <= self.max_bit_count(),
      "Too many bits written at once"
    );
    if bit_count == 0 {
      return Ok(());
    }

    if bit_count > 0 {
      self.buffer.extend(to_bits(bits, bit_count))
    }
    if self.buffer.len() > 512 {
      self.flush_buffer()?;
    }
    Ok(())
  }
  fn finalise(&mut self) -> std::io::Result<()> {
    if self.buffer.is_empty() {
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
    assert_eq!(u8::from(true), 1);
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
    writer.write_bits(0x0F1E_u16, 12).unwrap();
    writer.flush_buffer().unwrap();
    assert_bytes_eq!([0xF1], buf);
  }
  #[test]
  fn test_finalise_pads_last_byte_with_zeros() {
    let mut buf = Vec::new();
    let mut writer = BitwiseWriter::new(&mut buf);
    writer.write_bits(0xF_u16, 2).unwrap();
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
    assert!(!buf.is_empty());
  }
  #[test]
  fn test_doesnt_flush_after_a_single_bit_write() {
    let mut buf = Vec::new();
    let mut writer = BitwiseWriter::new(&mut buf);
    writer.write_bits(0xFF, 8).unwrap();
    assert_bytes_eq!([], buf);
  }
}
