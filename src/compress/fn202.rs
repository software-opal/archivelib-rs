use std::io::Read;

use crate::compress::{RCompressData, Result};
use crate::consts::CONST_N155_IS_8192;
use crate::support::BitwiseWrite;

const CHAR_BIT: usize = 8;
/// This was moved out of the `RCompressData` struct and into a constant as it was only changed in
///  the `finalize_compression` (`_197`) method.
/// 
/// This is calculated as the `_163.len()`(`= 8192`) minus the maximum number of entries that can be
///  added in the next iteration (8 calls * 3 bytes max) minus some extra padding for reasons
/// 
/// Obfuscated name: `->_183`
const BITS_BEFORE_FLUSH: usize = CONST_N155_IS_8192 - ((3 * CHAR_BIT) + 6);

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  /// Write a byte, run, or EOF into the byte run buffer.
  /// 
  /// This is called in three ways from the `compress` method:
  ///  - When writing a single byte: `(byte_value, 0)`
  ///  - When writing a run: `(0x100 + run_length - MIN_RUN_LEN, run_offset)`
  ///  - When writing EOF: `(0x100 + MAX_RUN_LEN + 1 - MIN_RUN_LEN, 0)`
  /// 
  /// With the variable's bounds being:
  ///  - `byte_value` falls in the bounds `0..=255`
  ///  - `run_length` falls in the bounds `MIN_RUN_LEN..=MAX_RUN_LEN`(`3..=256`)
  /// 
  /// The first argument is effectively a 9 bit number where the 9th bit is `0` if the byte is a raw
  ///  byte value; and `1` if it's a run or EOF.
  /// 
  /// These are stored in `_165` using the scheme described in that field's documentation. This
  ///  function also flushes the buffer when it fills up.
  /// 
  /// Obfuscated name: `void _202(ushort _203, ushort _204)`
  pub fn write_byte_or_run_into_buffer(&mut self, run_length_plus_256_or_byte: u16, run_offset_or_zero: u16) -> Result<()> {
    self.byte_run_length_buffer_counter >>= 1;
    if self.byte_run_length_buffer_counter == 0 {
      self.byte_run_length_buffer_counter = 1 << (CHAR_BIT - 1);
      if self.byte_or_run_buffer_index >= BITS_BEFORE_FLUSH {
        self.fn207_maybe_flush_state_to_output()?;
        self.byte_or_run_buffer_index = 0;
      }
      self.byte_run_length_buffer_bit_flag_index = self.byte_or_run_buffer_index;
      self.byte_or_run_buffer_index += 1;
      // This array item used to store which of the next 8 calls represent a run.
      // A `0` bit represents a raw byte; and a `1` bit represents a run(or EOF).
      self.byte_run_length_buffer[self.byte_run_length_buffer_bit_flag_index] = 0;
    }
    self.byte_or_run_buffer_index = write_byte_or_run_into_buffer(
      &mut self.byte_run_length_buffer,
      &mut self.byte_run_length_frequency,
      &mut self.run_offset_bit_count_frequency,
      self.byte_or_run_buffer_index,
      self.byte_run_length_buffer_bit_flag_index,
      self.byte_run_length_buffer_counter,
      run_length_plus_256_or_byte,
      run_offset_or_zero,
    );
    Ok(())
  }
}

#[allow(clippy::too_many_arguments)]
pub fn write_byte_or_run_into_buffer(
  byte_run_length_buffer: &mut [u8],
  byte_run_length_frequency: &mut [u16],
  run_offset_bit_count_frequency: &mut [u16],
  mut byte_or_run_buffer_index: usize,
  byte_run_length_buffer_bit_flag_index: usize,
  byte_run_length_buffer_counter: u16,
  run_length_plus_256_or_byte: u16,
  run_offset_or_zero: u16,
) -> usize {
  // Writes either:
  //  - the byte itself
  //  - the run length remapped to `0..=(256-3)`
  //  - the EOF flag(`0xFE` or `510-256`)
  byte_run_length_buffer[byte_or_run_buffer_index] = cast_trunc!(run_length_plus_256_or_byte as u8);
  byte_or_run_buffer_index += 1;

  // ??
  byte_run_length_frequency[cast!(run_length_plus_256_or_byte as usize)] += 1;
  if run_length_plus_256_or_byte >= (1 << CHAR_BIT) {
    // Either a run of bits, or EOF flag.

    // Set the bit representing this index in the bit flag index to indicate that this entry
    //  represents a run of bytes or the EOF
    byte_run_length_buffer[byte_run_length_buffer_bit_flag_index] |= cast!(byte_run_length_buffer_counter as u8);

    // Now write the the run offset into the next two array entries in little-endian order, so a run
    //  offset of `0x1234` is stored as `[0x34, 0x12]`
    byte_run_length_buffer[byte_or_run_buffer_index] = cast_trunc!(run_offset_or_zero as u8);
    byte_or_run_buffer_index += 1;
    byte_run_length_buffer[byte_or_run_buffer_index] = cast!((run_offset_or_zero >> 8) as u8);
    byte_or_run_buffer_index += 1;

    // while(_204){ _203++; _204>>=1; } _193[_203]++;
    let run_offset_bits = u16::BITS - run_offset_or_zero.leading_zeros();
    run_offset_bit_count_frequency[cast!(run_offset_bits as usize)] += 1;
  }
  byte_or_run_buffer_index
}
