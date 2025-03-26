#[macro_use]
mod array_alias;

mod base;
mod buffer;
#[allow(clippy::module_inception)]
mod compress;
mod config;
mod find_longest_run;
mod fn202;
mod fn207;
mod fn211;
mod fn216;
mod fn218;
mod fn222;
mod fn224;
mod fn225;
mod fn228;
mod fn230;

pub use self::base::{
  CompressError, CompressU8ArrayAlias, CompressU16ArrayAlias, RCompressData, Result,
};
pub use self::config::ArchivelibConfig;
use crate::CompressionLevel;

#[allow(dead_code)]
pub fn do_compress_level(
  input: &[u8],
  compression_level: CompressionLevel,
) -> std::result::Result<Box<[u8]>, std::string::String> {
  ArchivelibConfig::from(compression_level)
    .compress(input)
    .map_err(|err| format!("{}", err))
}

#[allow(dead_code)]
pub fn do_compress(input: &[u8]) -> std::result::Result<Box<[u8]>, std::string::String> {
  do_compress_level(input, CompressionLevel::Level0)
}
