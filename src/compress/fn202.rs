use std::io::Read;

use crate::compress::{RCompressData, Result};
use crate::consts::CONST_N155_IS_8192;
use crate::support::BitwiseWrite;

const CHAR_BIT: usize = 8;
/// This was moved out of the `RCompressData` struct and into a constant as it was only changed in
/// the `finalize_compression` (`_197`) method.
/// 
/// Obfuscated name: `->_183`
const BITS_BEFORE_FLUSH: usize = CONST_N155_IS_8192 - ((3 * CHAR_BIT) + 6);

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  ///
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
  /// The first argument is effectively a 9 bit number where the 9th bit is `0` if the byte is to be
  ///  read as-is; and `1` if it's a control character.
  /// 
  /// Obfuscated name: `void _202(ushort _203, ushort _204)`
  pub fn fn202(&mut self, run_length_plus_256_or_byte: u16, run_offset_or_zero: u16) -> Result<()> {
    self.bitwise_counter185 >>= 1;
    if self.bitwise_counter185 == 0 {
      self.bitwise_counter185 = cast!((1 << (CHAR_BIT - 1)) as u16);
      if self.array165_counter >= BITS_BEFORE_FLUSH {
        self.fn207()?;
        self.array165_counter = 0;
      }
      self.array165_tmp_counter186 = self.array165_counter;
      self.array165_counter += 1;
      self.dat_arr165[self.array165_tmp_counter186] = 0;
    }
    self.array165_counter = pure_fn202_part_2(
      &mut self.dat_arr165,
      &mut self.dat_arr191,
      &mut self.dat_arr193,
      self.array165_counter,
      self.array165_tmp_counter186,
      self.bitwise_counter185,
      run_length_plus_256_or_byte,
      run_offset_or_zero,
    );
    Ok(())
  }
}

#[allow(clippy::too_many_arguments)]
pub fn pure_fn202_part_2(
  dat_arr165: &mut [u8],
  dat_arr191: &mut [u16],
  dat_arr193: &mut [u16],
  mut array165_counter: usize,
  array165_tmp_counter186: usize,
  bitwise_counter185: u16,
  run_length_plus_256_or_byte: u16,
  run_offset_or_zero: u16,
) -> usize {
  // Writes the byte itself, the run length remapped to `0..=(256-3)`, or the EOF flag(`510`)
  dat_arr165[array165_counter] = cast_trunc!(run_length_plus_256_or_byte as u8);
  array165_counter += 1;
  dat_arr191[cast!(run_length_plus_256_or_byte as usize)] += 1;
  if run_length_plus_256_or_byte >= (1 << CHAR_BIT) {
    dat_arr165[array165_tmp_counter186] |= cast!(bitwise_counter185 as u8);
    dat_arr165[array165_counter] = cast_trunc!(run_offset_or_zero as u8);
    array165_counter += 1;
    dat_arr165[array165_counter] = cast!((run_offset_or_zero >> 8) as u8);
    array165_counter += 1;
    let counter = 16 - run_offset_or_zero.leading_zeros();
    dat_arr193[cast!(counter as usize)] += 1;
  }
  array165_counter
}
