use std::{collections::VecDeque, io::Write};

use crate::{
  consts::{MAX_COMPRESSION_FACTOR, MIN_COMPRESSION_FACTOR, MIN_RUN_LENGTH},
  consts_rewrite::{EOF_FLAG, MAX_RUN_LENGTH},
  support::BitwiseRead,
};

use super::{
  DecompressError, Result,
  buffer::ExpandHistoryBuffer,
  huffman_reader::{
    BitLengthTreeType, read_bit_length_tree, read_byte_frequency_tree, read_encoding,
  },
};

pub struct Extractor<R: BitwiseRead, W: Write> {
  reader: R,
  buffer: ExpandHistoryBuffer<W>,
}

impl<R: BitwiseRead, W: Write> Extractor<R, W> {
  pub fn new(reader: R, writer: W, compression_factor: u8) -> Result<Self> {
    if !(MIN_COMPRESSION_FACTOR..=MAX_COMPRESSION_FACTOR).contains(&compression_factor) {
      return Err(DecompressError::IllegalCompressionLevel(compression_factor));
    }
    let buffer_size = 1 << compression_factor;
    Ok(Self {
      reader,
      buffer: ExpandHistoryBuffer::new(writer, buffer_size),
    })
  }

  pub fn extract(&mut self) -> Result<()> {
    while self.extract_chunk()? {}

    Ok(())
  }

  fn extract_chunk(&mut self) -> Result<bool> {
    let lzss_entries = self.reader.read_bits(16)?;

    let byte_tree = read_byte_frequency_tree(&mut self.reader)?;
    let offset_tree = read_bit_length_tree(&mut self.reader, BitLengthTreeType::OffsetBitLength)?;

    if lzss_entries == 0 {
      return Ok(false);
    }

    for _ in 0..lzss_entries {
      let byte_or_run = read_encoding(&mut self.reader, &byte_tree)?;
      eprint!("ByteOrRun({})", byte_or_run);
      match byte_or_run {
        v @ 0..=255 => self.buffer.write_byte(cast!(v as u8))?,
        v @ 256..EOF_FLAG => {
          let run_len = v - 256 + MIN_RUN_LENGTH;

          let offset_bit_len = read_encoding(&mut self.reader, &offset_tree)?;
          let offset = match offset_bit_len {
            0 => 0,
            1 => 1,
            2..=16 => (1 << (offset_bit_len - 1)) | self.reader.read_bits(offset_bit_len - 1)?,
            17.. => unreachable!("Wat! {:?}", offset_tree),
          };
          eprint!("Offset({})", offset);

          if run_len > MAX_RUN_LENGTH {
            return Err(DecompressError::InvalidRunLength(run_len));
          }

          self.buffer.write_run(run_len, offset as usize)?;
        }
        EOF_FLAG => {
          self.set_eof();
          return Ok(false);
        }
        _ => unreachable!(),
      }
      eprintln!();
    }
    return Ok(true);
  }

  fn set_eof(&mut self) {}
}

#[cfg(test)]
mod test {
  use crate::support::BitBasedBitwiseReader;

  use super::*;

  #[test]
  fn test_read_bytes() {
    let reader = BitBasedBitwiseReader::from_hex_string(
      "
        00 11 43 49 B5 4F FA 0C  F2 06 E0 A8 39 01 FC 38
        18 3B 69 3A DA 5C DC 54  40 50 2A 32 55 9B 9F 0C
        FC FC
      ",
    );

    let mut output = vec![];
    let mut extractor = Extractor::new(reader, &mut output, 10).unwrap();

    extractor.extract().unwrap();

    assert_eq!(output, "I am what I am; ABABABAB".as_bytes());
  }
}
