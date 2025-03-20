use crate::{CompressError, compress::Result, support::BitwiseWrite};
use std::io::Read;

use super::{
  input_ring_buffer::{self, InputRingBuffer},
  lzss::{LzssBuffer, LzssEntry},
};

pub const MIN_COMPRESSION_FACTOR: u8 = 10;
pub const MAX_COMPRESSION_FACTOR: u8 = 14;
pub const MAX_RUN_LENGTH: usize = 256;

pub struct Compressor<R: Read, W: BitwiseWrite> {
  writer: W,

  input_file_ring_buffer: InputRingBuffer<R>,
  lzss_buffer: LzssBuffer,
}

impl<R: Read, W: BitwiseWrite> Compressor<R, W> {
  pub fn new(reader: R, writer: W, compression_factor: u8) -> Result<Self> {
    if !(MIN_COMPRESSION_FACTOR..=MAX_COMPRESSION_FACTOR).contains(&compression_factor) {
      return Err(CompressError::IllegalCompressionLevel(compression_factor));
    }
    Ok(Self {
      writer,
      input_file_ring_buffer: InputRingBuffer::new(reader, 1 << compression_factor),
      lzss_buffer: LzssBuffer::new(),
    })
  }

  pub fn compress(&mut self) -> Result<()> {
    // self.input_file_ring_buffer.initial_load();


    loop {
      self.input_file_ring_buffer.ensure_buffer_filled()?;
      let next_byte = if let Some(next_byte) = self.input_file_ring_buffer.next_byte() {
        next_byte
      } else {
        break;
      };

      if let Some((run, offset)) = self.input_file_ring_buffer.find_longest_run() {
        self.lzss_buffer.insert_element(LzssEntry::Run(run, offset));
        self.input_file_ring_buffer.advance_by(run);
      } else {
        self.lzss_buffer.insert_element(LzssEntry::Byte(next_byte));
        self.input_file_ring_buffer.advance_by(1);
      }
    }

    self.lzss_buffer.insert_element(LzssEntry::EoF);

    self.dump_lzss_buffer()?;

    Ok(())
  }

  fn dump_lzss_buffer(&mut self) -> Result<()> {
    Ok(())
  }
}

#[cfg(test)]
mod test {
  use crate::support::BitwiseWriter;

  use super::*;

  #[test]
  fn test_compress_abc() {
    let mut output = vec![];
    let mut compressor =
      Compressor::new("abc".as_bytes(), BitwiseWriter::new(&mut output), 10).unwrap();

    compressor.compress().unwrap();

    assert_eq!(
      compressor.lzss_buffer.data,
      [
        LzssEntry::Byte(97),
        LzssEntry::Byte(98),
        LzssEntry::Byte(99),
        LzssEntry::EoF
      ]
    );
  }

  #[test]
  fn test_compress_abcabc() {
    let mut output = vec![];
    let mut compressor =
      Compressor::new("abcabc".as_bytes(), BitwiseWriter::new(&mut output), 10).unwrap();

    compressor.compress().unwrap();

    assert_eq!(
      compressor.lzss_buffer.data,
      [
        LzssEntry::Byte(97),
        LzssEntry::Byte(98),
        LzssEntry::Byte(99),
        LzssEntry::Run(3, 2),
        LzssEntry::EoF
      ]
    );
  }
  #[test]
  fn test_compress_aaaabbbb() {
    let mut output = vec![];
    let mut compressor =
      Compressor::new("aaaabbbb".as_bytes(), BitwiseWriter::new(&mut output), 10).unwrap();

    compressor.compress().unwrap();

    assert_eq!(
      compressor.lzss_buffer.data,
      [
        LzssEntry::Byte(97),
        LzssEntry::Run(3, 0),
        LzssEntry::Byte(98),
        LzssEntry::Run(3, 0),
        LzssEntry::EoF
      ]
    );
  }

  #[test]
  fn test_compress_aaaa() {
    let mut output = vec![];
    let mut compressor =
      Compressor::new("aaaa".as_bytes(), BitwiseWriter::new(&mut output), 10).unwrap();

    compressor.compress().unwrap();

    assert_eq!(
      compressor.lzss_buffer.data,
      [LzssEntry::Byte(97), LzssEntry::Run(3, 0), LzssEntry::EoF]
    );
  }

  #[test]
  fn test_compress_1024_as_then_b() {
    let input = format!("{}b", ["a"; 1024].join(""));
    let mut output = vec![];
    let mut compressor =
      Compressor::new(input.as_bytes(), BitwiseWriter::new(&mut output), 10).unwrap();

    compressor.compress().unwrap();

    assert_eq!(
      compressor.lzss_buffer.data,
      [
        LzssEntry::Byte(97),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(255, 0),
        LzssEntry::Byte(98),
        LzssEntry::EoF
      ]
    );
  }
  #[test]
  fn test_compress_1024_as_then_4_bs() {
    let input = format!("{}bbbb", ["a"; 1024].join(""));
    let mut output = vec![];
    let mut compressor =
      Compressor::new(input.as_bytes(), BitwiseWriter::new(&mut output), 10).unwrap();

    compressor.compress().unwrap();

    assert_eq!(
      compressor.lzss_buffer.data,
      [
        LzssEntry::Byte(97),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(255, 0),
        LzssEntry::Byte(98),
        LzssEntry::Run(3, 0),
        LzssEntry::EoF
      ]
    );
  }

  #[test]
  fn test_compress_2048_as() {
    let input = ["a"; 2048].join("");
    let mut output = vec![];
    let mut compressor =
      Compressor::new(input.as_bytes(), BitwiseWriter::new(&mut output), 10).unwrap();

    compressor.compress().unwrap();

    assert_eq!(
      compressor.lzss_buffer.data,
      [
        LzssEntry::Byte(97),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(255, 0),
        LzssEntry::EoF
      ]
    );
  }
}
