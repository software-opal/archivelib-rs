#![deny(clippy::fallible_impl_from)]
#![deny(clippy::wrong_self_convention)]
#![deny(clippy::assertions_on_constants)]
#![deny(clippy::wrong_self_convention)]
#![deny(clippy::unseparated_literal_suffix)]
// Try to improve safety by requiring safer casts.
#![warn(clippy::cast_lossless)]
#![warn(clippy::cast_precision_loss)]
#![warn(clippy::cast_possible_wrap)]
#![warn(clippy::checked_conversions)]

#[macro_use]
pub mod support;

#[cfg(test)]
#[macro_use]
mod test;

pub mod compress;
pub mod expand;

mod consts;
mod huffman;
mod lzss;

mod level;

#[cfg(feature = "ported")]
pub mod ported;

pub use self::level::CompressionLevel;

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
pub use compress::do_compress_level;
pub use compress::do_compress_level_bitstream;

pub use expand::do_decompress;
pub use expand::do_decompress_level;
pub use expand::do_decompress_level_bitstream;
