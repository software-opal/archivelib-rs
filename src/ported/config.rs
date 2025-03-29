use std::io::{Read, Write};

use super::compress::RCompressData;
use super::errors::{CompressError, DecompressError};
use super::expand::RExpandData;
use super::support::BitReader;
use crate::level::CompressionLevel;
use crate::support::MaxSizeWriter;

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub struct ArchivelibConfig {
  pub level: CompressionLevel,
  pub max_size: Option<usize>,
}

impl Default for ArchivelibConfig {
  fn default() -> Self {
    Self {
      level: CompressionLevel::Level0,
      max_size: Some(65536),
    }
  }
}

impl From<CompressionLevel> for ArchivelibConfig {
  fn from(level: CompressionLevel) -> Self {
    Self {
      level,
      ..Self::default()
    }
  }
}

impl ArchivelibConfig {
  pub fn decompress(&self, input: &[u8]) -> Result<Box<[u8]>, DecompressError> {
    // Pre-allocate some memory to hold the decompressed stream; 16*input is arbitary.
    let mut out: Vec<u8> = Vec::with_capacity(256);
    match self.max_size {
      Some(limit) => {
        let mut writer = MaxSizeWriter::wrap(out, limit);
        self.decompress_stream(input, &mut writer)?;
        out = writer.into_inner();
      }
      None => {
        self.decompress_stream(input, &mut out)?;
      }
    }
    Ok(out.into_boxed_slice())
  }

  pub fn decompress_stream<R, W>(&self, input: R, output: W) -> Result<(), DecompressError>
  where
    R: Read,
    W: Write,
  {
    let reader = BitReader::from(input);
    let mut expander = RExpandData::new(reader, output, self.level.compression_factor())?;
    expander.expand()
  }

  pub fn compress(&self, input: &[u8]) -> Result<Box<[u8]>, CompressError> {
    // Pre-allocate some memory to hold the decompressed stream; 16*input is arbitary.
    let mut out: Vec<u8> = Vec::with_capacity(256);
    match self.max_size {
      Some(limit) => {
        let mut writer = MaxSizeWriter::wrap(out, limit);
        self.compress_stream(input, &mut writer)?;
        out = writer.into_inner();
      }
      None => {
        self.compress_stream(input, &mut out)?;
      }
    }
    Ok(out.into_boxed_slice())
  }
  pub fn compress_stream<R, W>(&self, input: R, output: W) -> Result<(), CompressError>
  where
    R: Read,
    W: Write,
  {
    let mut res =
      RCompressData::new_with_io_writer(input, output, self.level.compression_factor(), false)?;
    res.compress()
  }
}

#[cfg(test)]
mod tests {
  use super::*;

  #[test]
  fn test_decompress_no_writes() {
    let c = ArchivelibConfig {
      max_size: Some(1),
      ..ArchivelibConfig::default()
    };
    // compress([])
    let input = [0x00, 0x01, 0x00, 0x00, 0x1f, 0xe0, 0x00];
    let out = c.decompress(&input).unwrap();
    assert_eq!(out.into_vec(), vec![]);
  }

  #[test]
  #[ignore]
  fn test_decompress_fails_correctly_on_very_large_output() {
    let c = ArchivelibConfig {
      max_size: Some(1),
      ..ArchivelibConfig::default()
    };
    // SHA1: f0c957104bb1b80c9d125d9c8cbb3f06fbf2ab1a
    // Found by fuzzing, expands to [0;65537]
    let input = [0x00, 0x00, 0x00, 0x04];
    match c.decompress(&input) {
      Ok(out) => panic!(
        "Decompression did not respect the max_size attribute. Wrote {} bits instead.",
        out.len()
      ),
      Err(DecompressError::IOError(ioe)) => {
        match ioe.kind() {
          std::io::ErrorKind::WriteZero => {
            // This is the error we expect.
          }
          _ => panic!(
            "Decompression failed with an IOError other than the expected one: {:?}",
            ioe
          ),
        }
      }
      Err(other) => panic!("Decompression failed with an unexpected error: {:?}", other),
    };
  }
  #[test]
  #[ignore]
  fn test_decompress_works_on_output_exactly_max_size() {
    let c = ArchivelibConfig {
      // We shouldn't fail if the output is *exactly* the same as max_size
      max_size: Some(65537),
      ..ArchivelibConfig::default()
    };
    // SHA1: f0c957104bb1b80c9d125d9c8cbb3f06fbf2ab1a
    // Found by fuzzing, expands to [0;65537]
    let input = [0x00, 0x00, 0x00, 0x04];
    assert_eq!(c.decompress(&input).unwrap().into_vec(), vec![0; 65537])
  }
}
