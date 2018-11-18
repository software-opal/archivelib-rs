use crate::consts::{
  BUFFER_SIZE, CONST_N141_IS_511, CONST_N148_IS_4096, CONST_N149_IS_256, CONST_N152_IS_19,
  MAX_COMPRESSION_FACTOR, MIN_COMPRESSION_FACTOR,
};
use crate::support::{BitwiseRead, BitwiseWrite};

#[derive(Fail, Debug)]
pub enum ExpandError {
  #[fail(display = "Illegal Compression level: {}", _0)]
  IllegalCompressionLevel(u8),
}
pub type Result<R> = std::result::Result<R, ExpandError>;

#[derive(Clone)]
#[repr(C)]
pub struct RExpandData<R: BitwiseRead, W: BitwiseWrite> {
  pub input_store: R,
  pub output_store: W,
  pub uncompressed_buffer: Vec<u8>,
  pub dat_arr180: Vec<u8>,
  pub dat_arr181: Vec<u8>,
  pub dat_arr189: Vec<u16>,
  pub dat_arr190: Vec<u16>,
  pub dat_arr240: Vec<u16>,
  pub dat_arr241: Vec<u16>,
  pub compressed_data_buffer242: Vec<u8>,
  pub bits_in_buffer172: i16,
  pub max_uncompressed_data_size: i16,
  pub max_uncompressed_data_size_bitmask: i16,
  pub bits182: u16,
  pub error_counter243: i16,
  pub items_until_next_header: u16,
  pub tmp_bit_buffer245: u8,
  pub loaded_compressed_data_length246: i16,
  pub compressed_data_length248: isize,
}

impl<R: BitwiseRead, W: BitwiseWrite> RExpandData<R, W> {
  pub fn new(reader: R, writer: W, in_length: usize, compression_level: u8) -> Result<Self> {
    if compression_level > MAX_COMPRESSION_FACTOR || compression_level < MIN_COMPRESSION_FACTOR {
      Err(ExpandError::IllegalCompressionLevel(compression_level))
    } else {
      let max_size = 1 << compression_level;
      Ok(RExpandData {
        input_store: reader,
        output_store: writer,

        uncompressed_buffer: vec![0; max_size + 2],
        dat_arr180: vec![0; CONST_N141_IS_511],
        dat_arr181: vec![0; CONST_N152_IS_19],
        dat_arr189: vec![0; 2 * CONST_N141_IS_511 - 1],
        dat_arr190: vec![0; 2 * CONST_N141_IS_511 - 1],
        dat_arr240: vec![0; CONST_N148_IS_4096],
        dat_arr241: vec![0; CONST_N149_IS_256],
        compressed_data_buffer242: vec![0; BUFFER_SIZE],

        compressed_data_length248: in_length,
        max_uncompressed_data_size: max_size,
        max_uncompressed_data_size_bitmask: max_size - 1,
        error_counter243: 0,
        items_until_next_header: 0,
        bits182: 0,
        tmp_bit_buffer245: 0,
        bits_in_buffer172: 0,
        loaded_compressed_data_length246: 0,
      })
    }
  }
}
