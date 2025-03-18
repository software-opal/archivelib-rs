use std::io::Read;

use crate::compress::RCompressData;
use crate::consts::{CONST_N141_IS_511, CONST_N145_IS_19};
use crate::support::BitwiseWrite;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  /// Writes into `_217` based on data in the bit lengths.
  ///
  /// Combines the bit length of each value's encoding with the distance between encoded values.
  ///
  /// - `0` -- `0..=2` gap between values, or `19` gap.
  /// - `1` -- `3..=18` gap between values, or `19` gap.
  /// - `2` -- `20..` gap between values.
  /// - `3..` -- value bit length, with `1` bit length mapping to `3`.
  ///
  /// Obfuscated name: `void _216(ushort *_217);`
  pub fn build_byte_length_encoding_lengths(&mut self, var217_frequency_data: &mut [u16]) {
    for v in var217_frequency_data.iter_mut().take(CONST_N145_IS_19) {
      *v = 0;
    }
    // Find the first value in the table
    let mut largest_value: usize = CONST_N141_IS_511;
    while largest_value > 0
      && self.byte_run_length_huff_bit_length[cast!(largest_value as usize) - 1] == 0
    {
      largest_value -= 1
    }
    eprintln!("{:#05X}", largest_value);
    let mut idx: usize = 0;
    while idx < largest_value {
      let bit_length: usize = self.byte_run_length_huff_bit_length[idx] as usize;
      idx += 1;
      if bit_length == 0 {
        // The current index isn't in the output.
        let mut distance_to_next_value: i32 = 1;
        while (idx) < largest_value && self.byte_run_length_huff_bit_length[idx] == 0 {
          idx += 1;
          distance_to_next_value += 1
        }
        eprintln!("{}", distance_to_next_value);
        if distance_to_next_value <= 2 {
          var217_frequency_data[0] += cast!(distance_to_next_value as u16);
        } else if distance_to_next_value <= 18 {
          var217_frequency_data[1] += 1;
        } else if distance_to_next_value == 19 {
          var217_frequency_data[0] += 1;
          var217_frequency_data[1] += 1;
        } else {
          var217_frequency_data[2] += 1
        }
      } else {
        var217_frequency_data[bit_length + 2] += 1
      }
    }
  }
}
