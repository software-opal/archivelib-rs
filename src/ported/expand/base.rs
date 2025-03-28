use crate::consts::{
  CONST_N141_IS_511, CONST_N148_IS_4096, CONST_N149_IS_256, CONST_N152_IS_19,
  MAX_COMPRESSION_FACTOR, MIN_COMPRESSION_FACTOR,
};
pub use crate::errors::DecompressError;
use crate::support::BitRead;
use std::io::Write;
pub type Result<R> = std::result::Result<R, DecompressError>;

#[derive(Clone)]
#[repr(C)]
pub struct RExpandData<R: BitRead, W: Write> {
  pub input_store: R,
  pub output_store: W,
  pub uncompressed_buffer: Vec<u8>,
  pub dat_arr180: Vec<u8>,
  pub dat_arr181: Vec<u8>,
  pub dat_arr189: Vec<u16>,
  pub dat_arr190: Vec<u16>,
  pub dat_arr240: Vec<u16>,
  pub dat_arr241: Vec<u16>,
  pub bits_in_buffer172: i16, // usize?
  pub max_uncompressed_data_size: usize,
  pub max_uncompressed_data_size_bitmask: usize,
  pub bits182: u16,
  pub error_counter243: u8,
  pub items_until_next_header: usize,
  pub tmp_bit_buffer245: u8,
}

impl<R: BitRead, W: Write> RExpandData<R, W> {
  pub fn new(reader: R, writer: W, compression_level: u8) -> Result<Self> {
    assert_eq!(CONST_N141_IS_511, 511);
    if compression_level > MAX_COMPRESSION_FACTOR || compression_level < MIN_COMPRESSION_FACTOR {
      Err(DecompressError::IllegalCompressionLevel(compression_level))
    } else {
      let max_size = 1 << compression_level;
      Ok(RExpandData {
        input_store: reader,
        output_store: writer,

        uncompressed_buffer: vec![0; max_size],
        dat_arr180: vec![0; CONST_N141_IS_511],
        dat_arr181: vec![0; CONST_N152_IS_19],
        dat_arr189: vec![0; 2 * CONST_N141_IS_511 - 1],
        dat_arr190: vec![0; 2 * CONST_N141_IS_511 - 1],
        dat_arr240: vec![0; CONST_N148_IS_4096],
        dat_arr241: vec![0; CONST_N149_IS_256],

        max_uncompressed_data_size: max_size,
        max_uncompressed_data_size_bitmask: (max_size - 1),
        error_counter243: 0,
        items_until_next_header: 0,
        bits182: 0,
        tmp_bit_buffer245: 0,
        bits_in_buffer172: 0,
      })
    }
  }

  pub fn into_writer(self) -> W {
    self.output_store
  }
}
