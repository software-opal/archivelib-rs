mod byte_run_hash_table;
mod config;
mod error;
mod huffman_writer;
mod input_ring_buffer;
mod reader;
use std::io::Read;

pub use self::error::CompressError;
pub use self::reader::Compressor;
use crate::{CompressionLevel, support::BitwiseWrite};

pub type Result<T> = std::result::Result<T, CompressError>;

#[allow(dead_code)]
pub fn do_compress(input: &[u8]) -> std::result::Result<Box<[u8]>, std::string::String> {
  do_compress_level(input, CompressionLevel::Level0)
}

pub fn do_compress_level(
  input: &[u8],
  compression_level: CompressionLevel,
) -> std::result::Result<Box<[u8]>, std::string::String> {
  let mut arr = vec![];
  self::config::ArchivelibConfig::from(compression_level)
    .compress(input, &mut arr)
    .map_err(|err| format!("{}", err))
    .map(|_| arr.into_boxed_slice())
}

pub fn do_compress_level_bitstream(
  input: impl Read,
  writer: impl BitwiseWrite,
  compression_level: CompressionLevel,
) -> std::result::Result<(), std::string::String> {
  self::config::ArchivelibConfig::from(compression_level)
    .compress_bitstream(input, writer)
    .map_err(|err| format!("{}", err))
}
