mod byte_run_hash_table;
mod huffman_writer;
mod input_ring_buffer;
mod lzss;
mod reader;
mod config;

use crate::{  CompressionLevel,};

pub use self::reader::Compressor;
pub use self::config::ArchivelibConfig;


pub fn do_compress(
  input: &[u8],
) -> Result<Box<[u8]>, std::string::String> {
  do_compress_level(input, CompressionLevel::Level0)
}
pub fn do_compress_level(
  input: &[u8],
  compression_level: CompressionLevel,
) -> Result<Box<[u8]>, std::string::String> {
  let mut arr = vec![];
  self::config::ArchivelibConfig::from(compression_level)
    .compress(input, &mut arr)
    .map_err(|err| format!("{}", err))
    .map(|_| arr.into_boxed_slice())
}


