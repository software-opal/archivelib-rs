mod base;
mod buffer;
mod calculate_run_offset;
mod expand;
mod fn253;
mod fn255;
mod fn258;
mod get_next_item;

use crate::CompressionLevel;

pub use self::base::{RExpandData, Result};

use super::ArchivelibConfig;

pub fn do_decompress(input: &[u8]) -> std::result::Result<Box<[u8]>, std::string::String> {
  ArchivelibConfig::default()
    .decompress(input)
    .map_err(|err| format!("{}", err))
}

pub fn do_decompress_level(
  input: &[u8],
  compression_level: CompressionLevel,
) -> std::result::Result<Box<[u8]>, std::string::String> {
  (ArchivelibConfig {
    level: compression_level,
    ..ArchivelibConfig::default()
  })
  .decompress(input)
  .map_err(|err| format!("{}", err))
}
