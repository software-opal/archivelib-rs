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
    value_frequencies: &mut CompressU16ArrayAlias<'_>,
    dat_arr_cursor178: &mut CompressU8ArrayAlias<'_>,
    maybe_huff_value_to_idx: &mut CompressU16ArrayAlias<'_>,
  ) -> Result<u32> {
    let dat_arr_cursor178_offset = dat_arr_cursor178.offset(self);
    let dat_arr_cursor188_offset = maybe_huff_value_to_idx.offset(self);

    self.dat174_maybe_table_size = cast!(max_data_value as i16);
    let mut max_data_value_u32 = cast!(max_data_value as u32);
    // Note: 1-indexed.
    let mut remaining_items = 0;
    self.tmp_huffman_values_to_visit[1] = 0;
    for i in 0..cast!((self.dat174_maybe_table_size) as usize) {
      dat_arr_cursor178.set(self, i, 0);
      if 0 != value_frequencies.get(self, i) {
        remaining_items += 1;
        // Maps to the index in the frequency table with at least 1 entry
        self.tmp_huffman_values_to_visit[remaining_items] = i.try_into().unwrap();
      }
    }
    if remaining_items < 2 {
      // 0 or 1 values distinct values in the data(I.E 0 or 1 frequency entries).
      // Short circuit table setup.
      maybe_huff_value_to_idx.set(self, cast!((self.tmp_huffman_values_to_visit[1]) as usize), 0);
      Ok(self.tmp_huffman_values_to_visit[1].try_into().unwrap())
    } else {
      // `maybe_huff_used_values` starts at index 1 (index 0 appears unused).
      // Setup the `maybe_huff_used_values` to have the item with the smallest frequency at index 1
      let mut idx = (remaining_items / 2).try_into().unwrap();
      eprintln!("item_counter: {}; idx: {}", remaining_items, idx);
      // Moves the item in `maybe_huff_used_values` with the lowest frequency to the start of the list.
      while idx >= 1 {
        self.move_smallest_value_to_start(
          idx,
          value_frequencies,
          cast!(remaining_items as i16),
        );
        idx -= 1
      }

      // Now build the huffman table
      let mut branch_value;
      loop {
        // Grab the smallest value from the table.
        let smallest_value = self.tmp_huffman_values_to_visit[1];
        if smallest_value < self.dat174_maybe_table_size {
          maybe_huff_value_to_idx.set(self, 0, cast!(smallest_value as u16));
          maybe_huff_value_to_idx.shift(self, 1);
        }
        // Make the list shorter by moving the last item into the first item and then shortening the array.
        self.tmp_huffman_values_to_visit[1] = self.tmp_huffman_values_to_visit[remaining_items];
        remaining_items -= 1;

        // Now reshuffle the array to put the smallest item in index 1.
        self.move_smallest_value_to_start(1, value_frequencies, cast!(remaining_items as i16));

        // Grab the next smallest value out of the array.
        let next_smallest_value = self.tmp_huffman_values_to_visit[1];
        if next_smallest_value < self.dat174_maybe_table_size {
          maybe_huff_value_to_idx.set(self, 0, cast!(next_smallest_value as u16));
          maybe_huff_value_to_idx.shift(self, 1);
        }

        // Generate a branch node by using the values `max_data_value..`, and storing them in the
        //  frequency and used value table.
        branch_value = max_data_value_u32;
        max_data_value_u32 += 1;
        value_frequencies.set(
          self,
          cast!(branch_value as usize),
          value_frequencies.get(self, cast!(smallest_value as usize))
            + value_frequencies.get(self, cast!(next_smallest_value as usize)),
        );
        
        // Replace the `next_smallest_index` with the branch node, and move the next smallest
        //  element into the 1st position.
        self.tmp_huffman_values_to_visit[1] = cast!(branch_value as i16);
        self.move_smallest_value_to_start(1, value_frequencies, cast!(remaining_items as i16));

        // Write the branch information.
        self.dat_arr189_maybe_huff_left[cast!(branch_value as usize)] = cast!(smallest_value as u16);
        self.dat_arr190_maybe_huff_right[cast!(branch_value as usize)] = cast!(next_smallest_value as u16);
        if remaining_items <= 1 {
          break;
        }
      }
      maybe_huff_value_to_idx.set_offset(self, dat_arr_cursor188_offset);
      self.fn228(cast!(branch_value as i32), dat_arr_cursor178, maybe_huff_value_to_idx);
      maybe_huff_value_to_idx.set_offset(self, dat_arr_cursor188_offset);
      dat_arr_cursor178.set_offset(self, dat_arr_cursor178_offset);
      self.fn230(max_data_value, dat_arr_cursor178, maybe_huff_value_to_idx);
      Ok(branch_value)
    }
  }
}
