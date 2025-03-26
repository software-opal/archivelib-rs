use std::io::{Read, Write};

use crate::level::CompressionLevel;
use crate::support::{BitwiseReader, MaxSizeWriter};

use super::{DecompressError, Extractor};

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
    // Pre-allocate some memory to hold the decompressed stream.
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
    let reader = BitwiseReader::new(input);
    let mut extractor = Extractor::new(reader, output, self.level.compression_factor())?;
    extractor.extract()?;
    Ok(())
  }
}
