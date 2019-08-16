use super::array_alias::ArrayAlias;
use crate::consts::{
  CONST_N141_IS_511, CONST_N142_IS_15, CONST_N152_IS_19, CONST_N153_IS_4096, CONST_N155_IS_8192,
  MAX_COMPRESSION_FACTOR, MAX_RUN_LENGTH140, MIN_COMPRESSION_FACTOR,
};
use crate::support::{BitwiseWrite, BitwiseWriter};
use std::fmt;
use std::io::{Read, Write};

#[allow(dead_code)]
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
  pub enum<R: Read, W: BitwiseWrite> CompressU16ArrayAlias {
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
  pub enum<R: Read, W: BitwiseWrite> CompressU8ArrayAlias {
    type Parent = RCompressData<R, W>;
    type Item = u8;
    Array165 => dat_arr165;
    Array180 => dat_arr180;
    Array181 => dat_arr181;
  }
}

pub struct RCompressData<R: Read, W: BitwiseWrite> {
  pub input_store: R,
  pub output_store: W,
  pub dat_arr163: Vec<i16>,
  pub dat_arr164: Vec<i16>,
  pub dat_arr165: Vec<u8>,
  pub uncompressed_buffer: Vec<u8>,
  pub dat_arr167: Vec<u16>,
  pub dat_arr177: Vec<i16>,
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
  pub dat173: i16,
  pub dat174: i16,
  pub max_uncompressed_data_size: usize,
  pub max_uncompressed_data_size_bitmask: usize,
  // pub dat183_IS_CONST_8162: u16,
  pub array165_counter: usize,
  pub bitwise_counter185: u16,
  pub array165_tmp_counter186: usize,
}

fn vec_to_nice_debug<T: fmt::Debug + PartialEq>(v: &[T]) -> String {
  let mut base = "[".to_owned();
  if let Some(t) = v.first() {
    let mut last = t;
    let mut count = 0;
    for val in v {
      if val == last {
        count += 1;
      } else {
        if base.len() > 1 {
          base += ", ";
        }
        base += &match count {
          0 => "".to_owned(),
          1 => format!("{:?}", last),
          2 => format!("{:?}, {:?}", last, last),
          _ => format!("{:?} => {}", last, count),
        };
        last = val;
        count = 1;
      }
    }
    if base.len() > 1 {
      base += ", ";
    }
    base += &format!("{:?} => {}", last, count);
  }
  base + "]"
}

impl<R: Read, W: BitwiseWrite> fmt::Debug for RCompressData<R, W> {
  fn fmt(&self, formatter: &mut fmt::Formatter) -> fmt::Result {
    formatter
      .debug_struct("RCompressData")
      .field("dat_arr163", &vec_to_nice_debug(&self.dat_arr163))
      .field("dat_arr164", &vec_to_nice_debug(&self.dat_arr164))
      .field("dat_arr165", &vec_to_nice_debug(&self.dat_arr165))
      .field(
        "uncompressed_buffer",
        &vec_to_nice_debug(&self.uncompressed_buffer),
      )
      .field("dat_arr167", &vec_to_nice_debug(&self.dat_arr167))
      .field("dat_arr177", &vec_to_nice_debug(&self.dat_arr177))
      .field("dat_arr180", &vec_to_nice_debug(&self.dat_arr180))
      .field("dat_arr181", &vec_to_nice_debug(&self.dat_arr181))
      .field("dat_arr189", &vec_to_nice_debug(&self.dat_arr189))
      .field("dat_arr190", &vec_to_nice_debug(&self.dat_arr190))
      .field("dat_arr191", &vec_to_nice_debug(&self.dat_arr191))
      .field("dat_arr192", &vec_to_nice_debug(&self.dat_arr192))
      .field("dat_arr193", &vec_to_nice_debug(&self.dat_arr193))
      .field("dat_arr194", &vec_to_nice_debug(&self.dat_arr194))
      .field("chars_written", &self.chars_written)
      .field("input_length", &self.input_length)
      .field("uncompressible", &self.uncompressible)
      .field("fail_uncompressible", &self.fail_uncompressible)
      .field("dat168", &self.dat168)
      .field("dat169", &self.dat169)
      .field("dat173", &self.dat173)
      .field("dat174", &self.dat174)
      .field(
        "max_uncompressed_data_size",
        &self.max_uncompressed_data_size,
      )
      .field(
        "max_uncompressed_data_size_bitmask",
        &self.max_uncompressed_data_size_bitmask,
      )
      // .field("dat183_IS_CONST_8162", &self.dat183_IS_CONST_8162)
      .field("array165_counter", &self.array165_counter)
      .field("bitwise_counter185", &self.bitwise_counter185)
      .field("array165_tmp_counter186", &self.array165_tmp_counter186)
      .finish()
  }
}

impl<R: Read, W: Write> RCompressData<R, BitwiseWriter<W>> {
  pub fn new_with_io_writer(
    reader: R,
    writer: W,
    input_length: usize,
    compression_level: u8,
    fail_uncompressible: bool,
  ) -> Result<Self> {
    Self::new(
      reader,
      BitwiseWriter::new(writer),
      input_length,
      compression_level,
      fail_uncompressible,
    )
  }
}

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
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
      for v in dat_arr163.iter_mut().skip(max_size) {
        *v = -1;
      }

      Ok(RCompressData {
        input_store: reader,
        output_store: writer,
        fail_uncompressible,
        input_length,

        dat_arr163,
        dat_arr164: vec![-1; max_size],
        dat_arr165: vec![0; CONST_N155_IS_8192],
        uncompressed_buffer: vec![0; max_size + MAX_RUN_LENGTH140 + 2],
        dat_arr167: vec![0; 17],
        dat_arr177: vec![0; CONST_N141_IS_511 + 1],
        dat_arr180: vec![0; CONST_N141_IS_511],
        dat_arr181: vec![0; CONST_N152_IS_19],
        dat_arr189: vec![0; 2 * CONST_N141_IS_511 - 1],
        dat_arr190: vec![0; 2 * CONST_N141_IS_511 - 1],
        dat_arr191: vec![0; 2 * CONST_N141_IS_511 - 1],
        dat_arr192: vec![0; CONST_N141_IS_511],
        dat_arr193: vec![0; 2 * CONST_N142_IS_15 - 1],
        dat_arr194: vec![0; CONST_N152_IS_19],

        max_uncompressed_data_size: max_size,
        max_uncompressed_data_size_bitmask: (max_size - 1),
        chars_written: 0,

        uncompressible: false,
        dat168: 0,
        dat169: 0,
        dat173: 0,
        dat174: 0,
        // dat183_IS_CONST_8162: CONST_N155_IS_8192 as u16 - ((3 * 8) + 6),
        array165_counter: 0,
        bitwise_counter185: 0,
        array165_tmp_counter186: 0,
      })
    }
  }

  pub fn into_writer(self) -> W {
    self.output_store
  }
}
