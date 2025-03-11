use std::fmt;
use std::io::{Read, Write};

use super::array_alias::ArrayAlias;
use crate::consts::{
  BYTE_RUN_HASH_SIZE, CONST_N141_IS_511, CONST_N142_IS_15, CONST_N152_IS_19, CONST_N155_IS_8192,
  MAX_COMPRESSION_FACTOR, MAX_RUN_LENGTH, MIN_COMPRESSION_FACTOR,
};
pub use crate::errors::CompressError;
use crate::support::{BitwiseWrite, BitwiseWriter};

pub type Result<R> = std::result::Result<R, CompressError>;

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
  /// Obfuscated name: _161
  pub input_store: R,
  /// Obfuscated name: _162
  pub output_store: W,
  /// A hash table used to look up the starting indexes based on a 3-byte rolling hash.
  /// Used to reduce the search space when trying to find the index of a matching byte sequence.
  ///
  /// For data `max_size..`(key: `byte_hash + max_size`)
  ///   - `-1` means no hash has been found
  ///   - `index` means the hash was found at `index`.
  /// For data `..max_size`(key: `index`)
  ///   - `-1` means no other instances of this index's hash exist.
  ///   - `index` means there is at least 1 more instance found at `index`.
  ///
  /// To find all the references the code would look like this:
  /// ```rust
  /// fn find_all_hash_indexes(byte_run_hash_table: &[i16], byte_hash: usize) -> Vec<usize> {
  ///   let mut found_indexes = vec![];
  ///   let mut last_index = byte_run_hash_table[byte_hash];
  ///   while last_index >= 0 {
  ///     found_indexes.push(last_index as usize);
  ///     last_index = byte_run_hash_table[byte_hash]
  ///   }
  ///   return found_indexes;
  /// }
  /// ```
  ///
  /// Obfuscated name: _163
  pub byte_run_hash_table: Vec<i16>,
  /// Used to store what the 3-byte rolling hash was at a paticular offset. Allows us to remove
  ///  entries from `byte_run_hash_table` when we overwrite that position with further data from the
  ///  file.
  ///
  /// Whenever we load a byte over a previous byte in our rolling buffer we need to clear that index
  ///  from the `byte_run_hash_table`. To do this we store the hash of the index in this buffer,
  ///  that way we can quickly clear it from the hash table in O(1) time.
  ///
  /// Obfuscated name: _164
  pub buffer_offset_byte_hash: Vec<i16>,
  /// Obfuscated name: _165
  pub dat_arr165: Vec<u8>,
  /// A rolling buffer containing raw uncompressed data read from the data source. Length is
  ///  `max_uncompressed_data_size + MAX_RUN_LENGTH + 2` ( i.e. `data_size + 258`).
  ///
  /// The bytes from `0..MAX_RUN_LENGTH` are copied into `max_uncompressed_data_size..` when reading
  ///  more than `max_uncompressed_data_size` bytes, presumably to make the run detection code
  ///  simpler.
  ///
  /// Not sure why +2 though, maybe to fix an out of bounds access?
  ///
  /// Obfuscated name: _166
  pub uncompressed_buffer: Vec<u8>,
  /// Obfuscated name: _167
  pub dat_arr167: Vec<u16>,
  /// Obfuscated name: _177
  pub dat_arr177: Vec<i16>,
  /// Obfuscated name: _180
  pub dat_arr180: Vec<u8>,
  /// Obfuscated name: _181
  pub dat_arr181: Vec<u8>,
  /// Obfuscated name: _189
  pub dat_arr189: Vec<u16>,
  /// Obfuscated name: _190
  pub dat_arr190: Vec<u16>,
  /// Obfuscated name: _191
  pub dat_arr191: Vec<u16>,
  /// Obfuscated name: _192
  pub dat_arr192: Vec<u16>,
  /// Obfuscated name: _193
  pub dat_arr193: Vec<u16>,
  /// Obfuscated name: _194
  pub dat_arr194: Vec<u16>,
  // pub dat_arr_cursor178: Option<CompressU8ArrayAlias>,
  // pub dat_arr_cursor187: Option<CompressU16ArrayAlias>,
  // pub dat_arr_cursor188: Option<CompressU16ArrayAlias>,
  pub chars_written: usize,
  pub uncompressible: bool,
  pub fail_uncompressible: bool,
  pub longest_run: i16,
  pub longest_run_offset: i16,
  pub dat173: i16,
  pub dat174: i16,
  /// ZLib: `w_size`
  pub max_uncompressed_data_size: usize,
  /// ZLib: `w_mask`
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
      .field("dat_arr163", &vec_to_nice_debug(&self.byte_run_hash_table))
      .field(
        "dat_arr164",
        &vec_to_nice_debug(&self.buffer_offset_byte_hash),
      )
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
      .field("uncompressible", &self.uncompressible)
      .field("fail_uncompressible", &self.fail_uncompressible)
      .field("dat168", &self.longest_run)
      .field("dat169", &self.longest_run_offset)
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
    compression_level: u8,
    fail_uncompressible: bool,
  ) -> Result<Self> {
    Self::new(
      reader,
      BitwiseWriter::new(writer),
      compression_level,
      fail_uncompressible,
    )
  }
}

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn new(
    reader: R,
    writer: W,
    compression_level: u8,
    fail_uncompressible: bool,
  ) -> Result<Self> {
    if !(MIN_COMPRESSION_FACTOR..=MAX_COMPRESSION_FACTOR).contains(&compression_level) {
      Err(CompressError::IllegalCompressionLevel(compression_level))
    } else {
      // Compression Level is equivalent to MAX_WBITS in ZLib
      // Max Size is the window size in ZLib
      // This increases the amount of data we have to search for matches.
      let max_size = 1 << compression_level;

      let mut byte_run_hash_table = vec![0; max_size];
      // Setup the hash table's "hash" lookups with `-1` to indicate no value.
      byte_run_hash_table.extend_from_slice(&[-1; BYTE_RUN_HASH_SIZE]);
      debug_assert_eq!(byte_run_hash_table.len(), max_size + BYTE_RUN_HASH_SIZE);

      Ok(Self {
        input_store: reader,
        output_store: writer,
        fail_uncompressible,

        byte_run_hash_table,
        buffer_offset_byte_hash: vec![-1; max_size],
        dat_arr165: vec![0; CONST_N155_IS_8192],
        uncompressed_buffer: vec![0; max_size + MAX_RUN_LENGTH + 2],
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
        longest_run: 0,
        longest_run_offset: 0,
        dat173: 0,
        dat174: 0,
        // dat183_IS_CONST_8162: cast!(CONST_N155_IS_8192 as u16) - ((3 * 8) + 6),
        array165_counter: 0,
        bitwise_counter185: 0,
        array165_tmp_counter186: 0,
      })
    }
  }
}
