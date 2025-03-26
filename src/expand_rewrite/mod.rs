mod buffer;
mod config;
mod error;
mod huffman_reader;
mod reader;

use crate::CompressionLevel;

pub use config::ArchivelibConfig;
pub use error::DecompressError;
pub use reader::Extractor;

pub type Result<T> = std::result::Result<T, DecompressError>;

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
