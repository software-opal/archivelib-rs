use crate::consts::{
  BUFFER_SIZE, CONST_N141_IS_511, CONST_N142_IS_15, CONST_N152_IS_19, CONST_N153_IS_4096,
  CONST_N155_IS_8192, MAX_COMPRESSION_FACTOR, MAX_RUN_LENGTH140, MIN_COMPRESSION_FACTOR,
};
use crate::support::ArrayAlias;
use std::io::{Read, Write};

#[derive(Fail, Debug)]
pub enum CompressError {
  #[fail(display = "Illegal Compression level: {}", _0)]
  IllegalCompressionLevel(u8),
  #[fail(display = "Uncompressable")]
  InputUncompressable,
  #[fail(display = "Cursor {} Invariant failed", _0)]
  InvalidCursor(String),
  #[fail(display = "Invalid conversion: {}", error)]
  InvalidIntegerConversion {
    #[cause]
    error: std::num::TryFromIntError,
  },
  #[fail(display = "IOError: {}", error)]
  IOError {
    #[cause]
    error: std::io::Error,
  },
}
pub type Result<R> = std::result::Result<R, CompressError>;

impl From<std::num::TryFromIntError> for CompressError {
  fn from(v: std::num::TryFromIntError) -> Self {
    CompressError::InvalidIntegerConversion { error: v }
  }
}
impl From<std::io::Error> for CompressError {
  fn from(v: std::io::Error) -> Self {
    CompressError::IOError { error: v }
  }
}
array_alias_enum! {
  pub enum<R: Read, W: Write> CompressU16ArrayAlias {
    type Parent = RCompressData<R, W>;
    type Item = u16;
    Array167 => dat_arr167;
    Array189 => dat_arr189;
    Array190 => dat_arr190;
    Array191 => dat_arr191;
    Array192 => dat_arr192;
    Array193 => dat_arr193;
    Array194 => dat_arr194;
  }
  pub enum<R: Read, W: Write> CompressU8ArrayAlias {
    type Parent = RCompressData<R, W>;
    type Item = u8;
    Array165 => dat_arr165;
    Array180 => dat_arr180;
    Array181 => dat_arr181;
  }
}

pub struct RCompressData<R: Read, W: Write> {
  pub input_store: R,
  pub output_store: W,
  pub dat_arr163: Vec<i16>,
  pub dat_arr164: Vec<i16>,
  pub dat_arr165: Vec<u8>,
  pub uncompressed_buffer: Vec<u8>,
  pub dat_arr167: Vec<u16>,
  pub dat_arr177: Vec<i16>,
  pub buffer: Vec<u8>,
  pub dat_arr180: Vec<u8>,
  pub dat_arr181: Vec<u8>,
  pub dat_arr189: Vec<u16>,
  pub dat_arr190: Vec<u16>,
  pub dat_arr191: Vec<u16>,
  pub dat_arr192: Vec<u16>,
  pub dat_arr193: Vec<u16>,
  pub dat_arr194: Vec<u16>,
  // pub dat_arr_cursor178: Option<CompressU8ArrayAlias>,
  // pub dat_arr_cursor187: Option<CompressU16ArrayAlias>,
  // pub dat_arr_cursor188: Option<CompressU16ArrayAlias>,
  pub chars_written: usize,
  pub input_length: usize,
  pub uncompressible: bool,
  pub fail_uncompressible: bool,
  pub dat168: i16,
  pub dat169: i16,
  pub buffer_position: usize,
  pub bits_buffer_used172: u16,
  pub dat173: i16,
  pub dat174: i16,
  pub max_uncompressed_data_size: usize,
  pub max_uncompressed_data_size_bitmask: usize,
  pub bits_buffer182: u16,
  pub dat183_IS_CONST_8162: u16,
  pub array165_counter: u16,
  pub bitwise_counter185: u16,
  pub array165_tmp_counter186: u16,
}

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn new(
    reader: R,
    writer: W,
    input_length: usize,
    compression_level: u8,
    fail_uncompressible: bool,
  ) -> Result<Self> {
    if compression_level > MAX_COMPRESSION_FACTOR || compression_level < MIN_COMPRESSION_FACTOR {
      Err(CompressError::IllegalCompressionLevel(compression_level))
    } else {
      let max_size = 1 << compression_level;
      let dat_arr163_len = max_size + CONST_N153_IS_4096;

      let mut dat_arr163 = vec![0; dat_arr163_len];
      for i in max_size..dat_arr163.len() {
        dat_arr163[i] = -1;
      }

      Ok(RCompressData {
        input_store: reader,
        output_store: writer,
        fail_uncompressible: fail_uncompressible,
        input_length: input_length,

        dat_arr163: dat_arr163,
        dat_arr164: vec![-1; max_size],
        dat_arr165: vec![0; CONST_N155_IS_8192],
        uncompressed_buffer: vec![0; max_size + MAX_RUN_LENGTH140 + 2],
        dat_arr167: vec![0; 17],
        dat_arr177: vec![0; CONST_N141_IS_511 + 1],
        buffer: vec![0; BUFFER_SIZE],
        dat_arr180: vec![0; CONST_N141_IS_511],
        dat_arr181: vec![0; CONST_N152_IS_19],
        dat_arr189: vec![0; 2 * CONST_N141_IS_511 - 1],
        dat_arr190: vec![0; 2 * CONST_N141_IS_511 - 1],
        dat_arr191: vec![0; 2 * CONST_N141_IS_511 - 1],
        dat_arr192: vec![0; CONST_N141_IS_511],
        dat_arr193: vec![0; 2 * CONST_N142_IS_15 - 1],
        dat_arr194: vec![0; CONST_N152_IS_19],

        // dat_arr_cursor178: None,
        // dat_arr_cursor187: None,
        // dat_arr_cursor188: None,
        max_uncompressed_data_size: max_size,
        max_uncompressed_data_size_bitmask: (max_size - 1),
        chars_written: 0,

        uncompressible: false,
        dat168: 0,
        dat169: 0,
        buffer_position: 0,
        bits_buffer_used172: 0,
        dat173: 0,
        dat174: 0,
        bits_buffer182: 0,
        dat183_IS_CONST_8162: CONST_N155_IS_8192 as u16 - ((3 * 8) + 6),
        array165_counter: 0,
        bitwise_counter185: 0,
        array165_tmp_counter186: 0,
      })
    }
  }

  pub fn into_writer(self) -> W {
    return self.output_store;
  }
}
