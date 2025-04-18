use super::Result;
use crate::{
  CompressionLevel,
  huffman::{
    builder::frequency::{RootNode, build_from_frequency},
    sorts::SortAlgorithm,
  },
  lzss::{LzssBuffer, LzssEntry, Output},
  support::BitwiseWrite,
};
use std::io::Read;

use super::{
  huffman_writer::{write_bit_length_tree, write_byte_frequency_tree},
  input_ring_buffer::InputRingBuffer,
};

pub struct Compressor<R: Read, W: BitwiseWrite, S: SortAlgorithm> {
  writer: W,

  sort_algorithm: S,
  input_file_ring_buffer: InputRingBuffer<R>,
  lzss_buffer: LzssBuffer,
}

impl<R: Read, W: BitwiseWrite, S: SortAlgorithm> Compressor<R, W, S> {
  pub fn new(reader: R, writer: W, compression_level: CompressionLevel, sort_algorithm: S) -> Self {
    let compression_factor = compression_level.compression_factor();
    Self {
      writer,
      sort_algorithm,
      input_file_ring_buffer: InputRingBuffer::new(reader, 1 << compression_factor),
      lzss_buffer: LzssBuffer::new(),
    }
  }

  pub fn compress(&mut self) -> Result<()> {
    while self.fill_lzss_buffer()? {
      self.dump_lzss_buffer()?;
    }

    self.dump_lzss_buffer()?;

    self.writer.finalise()?;

    Ok(())
  }

  fn fill_lzss_buffer(&mut self) -> Result<bool> {
    while !self.lzss_buffer.is_full() {
      self.input_file_ring_buffer.ensure_buffer_filled()?;
      let next_byte = if let Some(next_byte) = self.input_file_ring_buffer.next_byte() {
        next_byte
      } else {
        self.lzss_buffer.insert_element(LzssEntry::EoF);

        return Ok(false);
      };

      if let Some((run, offset)) = self.input_file_ring_buffer.find_longest_run() {
        self.lzss_buffer.insert_element(LzssEntry::Run(run, offset));
        self.input_file_ring_buffer.advance_by(run);
      } else {
        self.lzss_buffer.insert_element(LzssEntry::Byte(next_byte));
        self.input_file_ring_buffer.advance_by(1);
      }
    }
    Ok(true)
  }

  fn dump_lzss_buffer(&mut self) -> Result<()> {
    let (lzss_byte_freq, lzss_offset_bitlen_freq) = self.lzss_buffer.generate_frequency_tables();

    let byte_encoding: (RootNode, Vec<(u16, usize)>) =
      build_from_frequency(&lzss_byte_freq, &self.sort_algorithm).unwrap();

    let mut output = vec![(byte_encoding.0.frequency(), 16)];

    output.append(&mut write_byte_frequency_tree(
      &byte_encoding.0,
      &byte_encoding.1,
      &self.sort_algorithm,
    ));

    let offset_bitlen_encoding =
      build_from_frequency(&lzss_offset_bitlen_freq, &self.sort_algorithm).unwrap();

    output.append(&mut write_bit_length_tree(
      &offset_bitlen_encoding.0,
      &offset_bitlen_encoding.1,
      super::huffman_writer::BitLengthTreeType::OffsetBitLength,
    ));

    output.extend(
      self
        .lzss_buffer
        .drain_as_output()
        .map(|output| match output {
          Output::ByteEncoded(byte) => byte_encoding.1[byte],
          Output::OffsetEncoded(offset_bits) => offset_bitlen_encoding.1[offset_bits],
          Output::Bits(bits, bit_len) => (bits, bit_len),
        }),
    );

    for (bits, bit_len) in output {
      self.writer.write_bits(bits, bit_len)?;
    }

    Ok(())
  }
}

#[cfg(test)]
mod test {
  use super::*;
  use crate::{ARCHIVE_LIB_SORT_ALGORITHM, support::BitwiseWriter};
  use proptest::prelude::*;

  proptest! {
  #[test]
  fn test_short_data(data in prop::array::uniform8(0_u8..)) {
    let mut output = vec![];
    let mut compressor = Compressor::new(
      &data[..],
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    compressor.compress().unwrap();
  }
  #[test]
  fn test_long_repeating_data(data in prop::array::uniform8(0_u8..), repeats in 1_usize..0x1_00) {
    let mut input = Vec::with_capacity(repeats * data.len());
    for _ in 0..repeats {
      input.extend(data);
    }
    let mut output = vec![];
    let mut compressor = Compressor::new(
      &input[..],
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    compressor.compress().unwrap();
  }
  }

  #[test]
  fn test_compress_long_run_of_data() {
    let input = ["aaaaaaab"; 33].join("");
    let mut output = vec![];
    let mut compressor = Compressor::new(
      input.as_bytes(),
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    compressor.compress().unwrap();
  }

  #[test]
  fn test_compress_very_long_file() {
    let mut input = vec![];
    for _ in 0..11388 {
      input.extend_from_slice(&[0, 0, 228, 154, 0, 0, 0, 0]);
    }
    let mut output = vec![];
    let mut compressor = Compressor::new(
      &input[..],
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    compressor.compress().unwrap();
  }

  #[test]
  fn test_lzss_offsets_full_output() {
    let mut output = vec![];
    let input = [0x00_u8, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00];
    let mut compressor = Compressor::new(
      &input[..],
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    compressor.compress().unwrap();

    assert_bytes_eq!(
      [
        0x00, 0x05, 0x28, 0x05, 0x3F, 0xF8, 0x49, 0x2C, 0xA7, 0x4C, 0x84, 0x02, 0x46, 0x98
      ],
      output
    );
  }

  #[test]
  fn test_lzss_offsets() {
    let mut output = vec![];
    let input = [0x00_u8, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00];
    let mut compressor = Compressor::new(
      &input[..],
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    assert!(!compressor.fill_lzss_buffer().unwrap());

    assert_eq!(
      [
        LzssEntry::Byte(0x00),
        LzssEntry::Run(0x03, 0),
        LzssEntry::Byte(0x39),
        LzssEntry::Run(0x03, 4),
        LzssEntry::EoF
      ],
      *compressor.lzss_buffer.data,
    );
  }

  #[test]
  fn test_compress_an_empty_file() {
    let mut output = vec![];
    let mut compressor = Compressor::new(
      "".as_bytes(),
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    assert!(!compressor.fill_lzss_buffer().unwrap());

    assert_eq!(output, []);
  }

  #[test]
  fn test_compress_abc() {
    let mut output = vec![];
    let mut compressor = Compressor::new(
      "abc".as_bytes(),
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    assert!(!compressor.fill_lzss_buffer().unwrap());

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
    let mut compressor = Compressor::new(
      "abcabc".as_bytes(),
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    assert!(!compressor.fill_lzss_buffer().unwrap());

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
    let mut compressor = Compressor::new(
      "aaaabbbb".as_bytes(),
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    assert!(!compressor.fill_lzss_buffer().unwrap());

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
    let mut compressor = Compressor::new(
      "aaaa".as_bytes(),
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    assert!(!compressor.fill_lzss_buffer().unwrap());

    assert_eq!(
      compressor.lzss_buffer.data,
      [LzssEntry::Byte(97), LzssEntry::Run(3, 0), LzssEntry::EoF]
    );
  }

  #[test]
  fn test_compress_1024_as_then_b() {
    let input = format!("{}b", ["a"; 1024].join(""));
    let mut output = vec![];
    let mut compressor = Compressor::new(
      input.as_bytes(),
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    assert!(!compressor.fill_lzss_buffer().unwrap());

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
    let mut compressor = Compressor::new(
      input.as_bytes(),
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    assert!(!compressor.fill_lzss_buffer().unwrap());

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
    let input = ["a"; 4096].join("");
    let mut output = vec![];
    let mut compressor = Compressor::new(
      input.as_bytes(),
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    assert!(!compressor.fill_lzss_buffer().unwrap());

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
        LzssEntry::Run(256, 0),
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

  #[test]
  fn test_compress_4096_as_then_4096_bs() {
    let input = ["a"; 4096].join("") + &["b"; 4096].join("");
    let mut output = vec![];
    let mut compressor = Compressor::new(
      input.as_bytes(),
      BitwiseWriter::new(&mut output),
      CompressionLevel::Level0,
      ARCHIVE_LIB_SORT_ALGORITHM,
    );

    assert!(!compressor.fill_lzss_buffer().unwrap());

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
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(255, 0),
        LzssEntry::Byte(98),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
        LzssEntry::Run(256, 0),
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
