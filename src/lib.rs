#![deny(clippy::fallible_impl_from)]
#![deny(clippy::wrong_pub_self_convention)]
#![deny(clippy::assertions_on_constants)]
#![deny(clippy::wrong_self_convention)]
#![deny(clippy::unseparated_literal_suffix)]
// Try to improve safety by requiring safer casts.
#![warn(clippy::cast_lossless)]
#![warn(clippy::cast_precision_loss)]
#![warn(clippy::cast_possible_wrap)]
#![warn(clippy::checked_conversions)]

#[macro_use]
mod support;

#[cfg(test)]
#[macro_use]
mod test;

mod compress_rewrite;
mod consts_rewrite;
mod expand_rewrite;

mod huffman;
mod lzss;

mod level;

#[cfg(not(feature = "new_impl"))]
mod expand;
#[cfg(feature = "new_impl")]
mod expand_new;

mod compress;
mod config;
mod consts;
mod errors;

pub use self::config::ArchivelibConfig;
pub use self::errors::*;
pub use self::level::CompressionLevel;

pub use compress_rewrite::Compressor;
pub use huffman::builder::frequency::build_from_frequency;
pub use huffman::sorts::{
  ARCHIVE_LIB_SORT_ALGORITHM, ArchiveLibSortAlgorithm, MODERN_SORT_ALGORITHM, ModernSortAlgorithm,
  SortAlgorithm,
};

#[cfg(feature = "sys")]
pub mod sys {
  pub use archivelib_sys::{do_compress, do_compress_level, do_decompress, do_decompress_level};
}

pub fn do_compress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  do_compress_level(input, CompressionLevel::Level0)
}

#[cfg(not(feature = "new_impl"))]
pub use compress::do_compress_level;
#[cfg(feature = "new_impl")]
pub use compress_rewrite::do_compress_level;

pub fn do_decompress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  ArchivelibConfig::default()
    .decompress(input)
    .map_err(|err| format!("{}", err))
}

pub fn do_decompress_level(
  input: &[u8],
  compression_level: CompressionLevel,
) -> Result<Box<[u8]>, std::string::String> {
  (ArchivelibConfig {
    level: compression_level,
    ..ArchivelibConfig::default()
  })
  .decompress(input)
  .map_err(|err| format!("{}", err))
}
