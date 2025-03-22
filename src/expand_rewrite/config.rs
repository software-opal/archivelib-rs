use std::io::{Read, Write};

use crate::errors::DecompressError;
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
  #[cfg(not(feature = "new_impl"))]
  pub fn decompress_stream<R, W>(&self, input: R, output: W) -> Result<(), DecompressError>
  where
    R: Read,
    W: Write,
  {
    use crate::expand;
    use crate::support::BitReader;

    let mut reader = BitReader::from(input);
    let mut expander = expand::RExpandData::new(reader, output, self.level.compression_factor())?;
    expander.expand()
  }
  #[cfg(feature = "new_impl")]
  pub fn decompress_stream<R, W>(&self, input: R, mut output: W) -> Result<(), DecompressError>
  where
    R: Read,
    W: Write,
  {
    use crate::expand_new;
    use crate::support::CorrectLookAheadBitwiseReader;

    let mut reader = CorrectLookAheadBitwiseReader::from_reader(input);
    expand_new::expand(&mut reader, &mut output, self.level)
  }
  }