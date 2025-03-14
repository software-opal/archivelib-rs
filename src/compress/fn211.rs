use std::convert::TryInto;
use std::io::Read;

use super::array_alias::ArrayAlias;
use crate::compress::{CompressU8ArrayAlias, CompressU16ArrayAlias, RCompressData, Result};
use crate::support::BitwiseWrite;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  /// 
  /// ZLib: `build_tree`
  /// Obfuscated name: `int _211(int _212, ushort *_213, uchar *_214, ushort *_215)`
  pub fn fn211_maybe_build_huffman_table(
    &mut self,
    max_data_value: i32,
    // array aliases act like a mutable pointer to an array pointer.
    // The act like an array, but also allow changing which index is the start of the array.
    // We need to do this to allow us to mutate these arrays across multiple methods.
    // On the future I'll probably rewrite the huffman table generation to be more rust-y.
    maybe_frequency_table: &mut CompressU16ArrayAlias<'_>,
    dat_arr_cursor178: &mut CompressU8ArrayAlias<'_>,
    maybe_huff_value_to_idx: &mut CompressU16ArrayAlias<'_>,
  ) -> Result<u32> {
    let dat_arr_cursor178_offset = dat_arr_cursor178.offset(self);
    let dat_arr_cursor188_offset = maybe_huff_value_to_idx.offset(self);

    self.dat174_maybe_table_size = cast!(max_data_value as i16);
    let mut max_data_value_u32 = cast!(max_data_value as u32);
    let mut item_counter = 0;
    self.maybe_huff_used_values[1] = 0;
    for i in 0..cast!((self.dat174_maybe_table_size) as usize) {
      dat_arr_cursor178.set(self, i, 0);
      if 0 != maybe_frequency_table.get(self, i) {
        item_counter += 1;
        // Maps to the index in the frequency table with at least 1 entry
        self.maybe_huff_used_values[item_counter] = i.try_into().unwrap();
      }
    }
    if item_counter < 2 {
      // 0 or 1 values distinct values in the data(I.E 0 or 1 frequency entries).
      // Short circuit table setup.
      maybe_huff_value_to_idx.set(self, cast!((self.maybe_huff_used_values[1]) as usize), 0);
      Ok(self.maybe_huff_used_values[1].try_into().unwrap())
    } else {
      // `maybe_huff_used_values` starts at index 1 (index 0 appears unused).
      // Setup the `maybe_huff_used_values` to have the item with the smallest frequency at index 1
      let mut idx = (item_counter / 2).try_into().unwrap();
      eprintln!("item_counter: {}; idx: {}", item_counter, idx);
      // Moves the item in `maybe_huff_used_values` with the lowest frequency to the start of the list.
      while idx >= 1 {
        self.fn225_maybe_move_smallest_item_to_start(
          idx,
          maybe_frequency_table,
          cast!(item_counter as i16),
        );
        idx -= 1
      }

      // Now build the huffman table
      let mut var289;
      loop {
        // Grab the smallest value from the table.
        let run_start226 = self.maybe_huff_used_values[1];
        if run_start226 < self.dat174_maybe_table_size {
          maybe_huff_value_to_idx.set(self, 0, cast!(run_start226 as u16));
          maybe_huff_value_to_idx.shift(self, 1);
        }
        self.maybe_huff_used_values[1] = self.maybe_huff_used_values[item_counter];
        item_counter -= 1;
        self.fn225_maybe_move_smallest_item_to_start(1, maybe_frequency_table, cast!(item_counter as i16));
        let run_length276 = self.maybe_huff_used_values[1];
        if run_length276 < self.dat174_maybe_table_size {
          maybe_huff_value_to_idx.set(self, 0, cast!(run_length276 as u16));
          maybe_huff_value_to_idx.shift(self, 1);
        }
        var289 = max_data_value_u32;
        max_data_value_u32 += 1;
        // Maybe a branch node generation?!
        maybe_frequency_table.set(
          self,
          cast!(var289 as usize),
          maybe_frequency_table.get(self, cast!(run_start226 as usize))
            + maybe_frequency_table.get(self, cast!(run_length276 as usize)),
        );
        self.maybe_huff_used_values[1] = cast!(var289 as i16);
        self.fn225_maybe_move_smallest_item_to_start(1, maybe_frequency_table, cast!(item_counter as i16));
        self.dat_arr189[cast!(var289 as usize)] = cast!(run_start226 as u16);
        self.dat_arr190[cast!(var289 as usize)] = cast!(run_length276 as u16);
        if item_counter <= 1 {
          break;
        }
      }

      maybe_huff_value_to_idx.set_offset(self, dat_arr_cursor188_offset);
      self.fn228(cast!(var289 as i32), dat_arr_cursor178, maybe_huff_value_to_idx);
      maybe_huff_value_to_idx.set_offset(self, dat_arr_cursor188_offset);
      dat_arr_cursor178.set_offset(self, dat_arr_cursor178_offset);
      self.fn230(max_data_value, dat_arr_cursor178, maybe_huff_value_to_idx);
      Ok(var289)
    }
  }
}
