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

// I needed to do this otherwise the compiler threw errors. What errors? Good question.
array_alias_enum! {
  pub enum<R: Read, W: BitwiseWrite> CompressU16ArrayAlias {
    type Parent = RCompressData<R, W>;
    type Item = u16;
    /// Obfuscated name: _167
    Array167 => dat_arr167;
    /// Obfuscated name: _189
    Array189 => dat_arr189;
    /// Obfuscated name: _190
    Array190 => dat_arr190;
    /// Obfuscated name: _191
    ByteRunLengthFrequency => byte_run_length_frequency;
    Array192 => dat_arr192;
    /// Obfuscated name: _193
    RunOffsetBitCountFrequency => run_offset_bit_count_frequency;
    /// Obfuscated name: _194
    Array194 => dat_arr194;
  }
  pub enum<R: Read, W: BitwiseWrite> CompressU8ArrayAlias {
    type Parent = RCompressData<R, W>;
    type Item = u8;
    /// Obfuscated name: _165
    ByteRunLengthBuffer => byte_run_length_buffer;
    /// Obfuscated name: _180
    Array180 => dat_arr180;
    /// Obfuscated name: _181
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
  /// This is a temporary buffer storing bytes and run lengths.
  /// 
  /// This is populated by `_202` and stores the following in 8-call chunks:
  ///  - `0` -- An 8-bit bit field, where the most significant bit represents the first item and the
  ///            least significant represents the 8th. Each bit means:
  ///     - `0` bit means the `_202` call at this index represented a raw byte; and a single item
  ///            was pushed onto `_165`
  ///     - `1` bit means the `_202` call at this index represented a run length or EOF flag; and 3
  ///            items were pushed onto `_165`
  ///  - `1..` -- One of the following:
  ///     - 1 byte representing a raw byte value.
  ///     - 3 bytes representing a run length and offset:
  ///        - 1 byte representing the `run_length - 3` (range `0..=0xFD`).
  ///        - 2 little-endian bytes representing the negative offset to the run. Note that this is
  ///             stored with the lowest bits in the first byte. So an offset of `0x1234` is
  ///             represented in this array as `[0x34, 0x12]`
  ///     - 3 bytes representing the EOF flag: `[0xFE, 0, 0]`
  /// 
  /// The filling of this array is supported by a counter(`_184`); the bit-field index(`_186`); and
  /// a bitwise counter(`_185`).
  /// 
  /// - `_184` is the next index to be written too.
  /// - `_185` is a bitwise counter(so counts `0x80`, `0x40`, etc.) that is bitwise-ORed with the
  ///           bit field at index `_186` if this call represents a run or EOF.
  /// - `_186` is index of the bit field (the `0` index in the 8-call chunk described above)
  /// 
  /// Obfuscated name: _165
  pub byte_run_length_buffer: Vec<u8>,
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
  pub maybe_huff_used_values: Vec<i16>,
  /// Obfuscated name: _180
  pub dat_arr180: Vec<u8>,
  /// Obfuscated name: _181
  pub dat_arr181: Vec<u8>,
  /// Obfuscated name: _189
  pub dat_arr189: Vec<u16>,
  /// Obfuscated name: _190
  pub dat_arr190: Vec<u16>,
  /// Stores the number of times a given byte or run length has been seen.
  /// 
  /// This stores values from `0..=510`, with `0..256` representing bytes, `256..510` representing
  ///  run lengths, and `510` representing the EOF flag. Each time a value is seen by `_202` the
  ///  respective index is incremented. 
  /// 
  /// This array is used by `_207` to write to the output buffer; and then `_207` will also clear
  ///  the array.
  /// 
  ///  Obfuscated name: _191
  pub byte_run_length_frequency: Vec<u16>,
  /// Obfuscated name: _192
  pub dat_arr192: Vec<u16>,
  /// Stores the frequency of different run length bit-lengths were seen.
  /// 
  /// This is written in `_202` by calculating the number of bits required to store the run length,
  ///  and then incrementing that number in this array. 
  /// 
  /// This array is used by `_207` to write to the output buffer; and then `_207` will also clear
  ///  the array.
  /// 
  /// Obfuscated name: _193
  pub run_offset_bit_count_frequency: Vec<u16>,
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
  pub dat174_maybe_table_size: i16,
  /// ZLib: `w_size`
  pub max_uncompressed_data_size: usize,
  /// ZLib: `w_mask`
  pub max_uncompressed_data_size_bitmask: usize,
  // pub dat183_IS_CONST_8162: u16,
  /// Obfuscated name: _184
  pub byte_or_run_buffer_index: usize,
  /// Counts `0x80` to `0x0` by left-shifting by one.
  /// 
  /// Possible values are `0x80`, `0x40`, `0x20`, `0x10`, `0x08`, `0x04`, `0x02`, `0x01`, `0x00`
  /// 
  /// Obfuscated name: _185
  pub byte_run_length_buffer_counter: u16,
  /// This represents the index in `_165` that stores the run length bit fields.
  /// 
  /// See `_165` for more details
  /// 
  /// Obfuscated name: _186
  pub byte_run_length_buffer_bit_flag_index: usize,
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
    base += &match count {
      0 => "".to_owned(),
      1 => format!("{:?}", last),
      2 => format!("{:?}, {:?}", last, last),
      _ => format!("{:?} => {}", last, count),
    };
  }
  base + "]"
}

impl<R: Read, W: BitwiseWrite> fmt::Debug for RCompressData<R, W> {
  fn fmt(&self, formatter: &mut fmt::Formatter) -> fmt::Result {
    formatter
      .debug_struct("RCompressData")
      .field("byte_run_hash_table", &vec_to_nice_debug(&self.byte_run_hash_table))
      .field(
        "buffer_offset_byte_hash",
        &vec_to_nice_debug(&self.buffer_offset_byte_hash),
      )
      .field("byte_run_length_buffer", &vec_to_nice_debug(&self.byte_run_length_buffer))
      .field(
        "uncompressed_buffer",
        &vec_to_nice_debug(&self.uncompressed_buffer),
      )
      .field("dat_arr167", &vec_to_nice_debug(&self.dat_arr167))
      .field("dat_arr177", &vec_to_nice_debug(&self.maybe_huff_used_values))
      .field("dat_arr180", &vec_to_nice_debug(&self.dat_arr180))
      .field("dat_arr181", &vec_to_nice_debug(&self.dat_arr181))
      .field("dat_arr189", &vec_to_nice_debug(&self.dat_arr189))
      .field("dat_arr190", &vec_to_nice_debug(&self.dat_arr190))
      .field("byte_run_length_frequency", &vec_to_nice_debug(&self.byte_run_length_frequency))
      .field("dat_arr192", &vec_to_nice_debug(&self.dat_arr192))
      .field("run_offset_bit_count_frequency", &vec_to_nice_debug(&self.run_offset_bit_count_frequency))
      .field("dat_arr194", &vec_to_nice_debug(&self.dat_arr194))
      .field("chars_written", &self.chars_written)
      .field("uncompressible", &self.uncompressible)
      .field("fail_uncompressible", &self.fail_uncompressible)
      .field("longest_run", &self.longest_run)
      .field("longest_run_offset", &self.longest_run_offset)
      .field("dat173", &self.dat173)
      .field("dat174", &self.dat174_maybe_table_size)
      .field(
        "max_uncompressed_data_size",
        &self.max_uncompressed_data_size,
      )
      .field(
        "max_uncompressed_data_size_bitmask",
        &self.max_uncompressed_data_size_bitmask,
      )
      // .field("dat183_IS_CONST_8162", &self.dat183_IS_CONST_8162)
      .field("byte_or_run_buffer_index", &self.byte_or_run_buffer_index)
      .field("byte_run_length_buffer_counter", &self.byte_run_length_buffer_counter)
      .field("byte_run_length_buffer_bit_flag_index", &self.byte_run_length_buffer_bit_flag_index)
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
        byte_run_length_buffer: vec![0; CONST_N155_IS_8192],
        uncompressed_buffer: vec![0; max_size + MAX_RUN_LENGTH + 2],
        dat_arr167: vec![0; 17],
        maybe_huff_used_values: vec![0; CONST_N141_IS_511 + 1],
        dat_arr180: vec![0; CONST_N141_IS_511],
        dat_arr181: vec![0; CONST_N152_IS_19],
        dat_arr189: vec![0; 2 * CONST_N141_IS_511 - 1],
        dat_arr190: vec![0; 2 * CONST_N141_IS_511 - 1],
        byte_run_length_frequency: vec![0; 2 * CONST_N141_IS_511 - 1],
        dat_arr192: vec![0; CONST_N141_IS_511],
        run_offset_bit_count_frequency: vec![0; 2 * CONST_N142_IS_15 - 1],
        dat_arr194: vec![0; CONST_N152_IS_19],

        max_uncompressed_data_size: max_size,
        max_uncompressed_data_size_bitmask: (max_size - 1),
        chars_written: 0,

        uncompressible: false,
        longest_run: 0,
        longest_run_offset: 0,
        dat173: 0,
        dat174_maybe_table_size: 0,
        // dat183_IS_CONST_8162: cast!(CONST_N155_IS_8192 as u16) - ((3 * 8) + 6),
        byte_or_run_buffer_index: 0,
        byte_run_length_buffer_counter: 0,
        byte_run_length_buffer_bit_flag_index: 0,
      })
    }
  }
}
