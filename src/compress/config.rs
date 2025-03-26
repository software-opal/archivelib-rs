use std::io::{Read, Write};

use crate::errors::CompressError;
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
  #![allow(dead_code)]

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
    use crate::compress;

    let mut res = compress::RCompressData::new_with_io_writer(
      input,
      output,
      self.level.compression_factor(),
      false,
    )?;
    res.compress()
  }
}
