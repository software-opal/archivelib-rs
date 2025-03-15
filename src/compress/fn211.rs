use std::convert::TryInto;
use std::io::Read;

use super::array_alias::ArrayAlias;
use crate::compress::{CompressU8ArrayAlias, CompressU16ArrayAlias, RCompressData, Result};
use crate::support::BitwiseWrite;
use crate::support::binary_tree_printer::print_tree;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  /// Builds a huffman tree in `_189` and `_190` and returns the root node's value.
  ///
  /// Returns the "value" of the tree's root node.
  ///
  /// Values in the range `0.._212` are "real values"; and `_212..` are binary tree nodes.
  ///
  /// The huffman trees are built from an array mapping `value`s to `frequency`. This is stored in
  ///  `value_frequencies` (`_213`).
  ///
  /// For example if `0x01` was seen 5 times; we would expect `value_frequencies[0x01] == 6`. And if
  ///  `0x03` was not seen then we would expect `value_frequencies[0x03] == 0`.
  ///
  /// We first find all the values with a non zero frequency, and store them in
  ///  `tmp_huffman_values_to_visit` (`_177`) starting with index `1`. So for a frequency table
  ///  `[0,5,4,0,1]` we would expected that list to equal `[0, 0x01, 0x02, 0x04]`.
  ///
  /// Then we call `_225` a number of times to update the `_177` array to put the value with the
  /// lowest frequency at the `1` index in the array. So we would expect the above array to have
  /// `0x04` at index 1.
  ///
  /// Then whilst we have more than 2 items in `_177`, we:
  /// - We then pop that lowest frequency value off the array. We do this by swapping index `1` with
  ///   the last item, and then shortening the array's length.
  /// - We append the lowest frequency value to the end of `_215`
  /// - We then call `_225` once to place the next lowest frequency item to the start of the array.
  /// - We append the 2nd lowest frequency value (now at position 1 in `_177` after the `_225` call)
  ///    to the end of `_215`
  /// - We then combine the lowest frequency value and the 2nd lowest frequency (now at position 1
  ///    in the `_177`) into a "node". This node's value starts at `max_data_value` (e.g. `0x1FF`
  ///    for byte data) and is incremented each time a node is created.
  /// - This node's value is placed at index 1, and the frequency is written to the frequency array
  ///    as the sum of it's two leaf node's frequencies. Then the lowest and 2nd lowest values are
  ///    written into the `_189[node_value]` and `_190[node_value]` respectively.
  ///
  /// This means that we've now got a root node, and the binary tree stored in `_189` and `_190`;
  ///  and we have an ascending order list of nodes in `_215`.
  ///
  /// We then calculate the depths of the huffman tree's nodes, storing them in `tree_value_depths`.
  ///  We then use the depths to assign both the node's depth, and it's huffman encoding, storing
  ///  the results in `tree_value_depths` and `values_in_tree`.
  /// 
  /// To determine the exact huffman encoding for a specific value we would look up the value's
  ///  encoding in `values_in_tree`, and the number of bits used for that encoding in
  ///  `tree_value_depths`.
  /// 
  /// ZLib: `build_tree`
  ///
  /// Obfuscated name: `int _211(int _212, ushort *_213, uchar *_214, ushort *_215)`
  pub fn build_huffman_encoding(
    &mut self,
    data_values_length: i32,
    // array aliases act like a mutable pointer to an array pointer.
    // The act like an array, but also allow changing which index is the start of the array.
    // We need to do this to allow us to mutate these arrays across multiple methods.
    // On the future I'll probably rewrite the huffman table generation to be more rust-y.
    value_frequencies: &mut CompressU16ArrayAlias<'_>,
    tree_value_depths: &mut CompressU8ArrayAlias<'_>,
    // This array contains all the nodes(except the root) in the tree in ascending frequency order.
    values_in_tree: &mut CompressU16ArrayAlias<'_>,
  ) -> Result<u32> {
    let dat_arr_cursor178_offset = tree_value_depths.offset(self);
    let values_in_tree_orig_offset = values_in_tree.offset(self);

    self.tmp_huffman_table_min_node_value = cast!(data_values_length as i16);
    let mut next_branch_idx = cast!(data_values_length as u32);
    // Note: 1-indexed.
    let mut remaining_items = 0;
    self.tmp_huffman_values_to_visit[1] = 0;
    for i in 0..cast!((self.tmp_huffman_table_min_node_value) as usize) {
      tree_value_depths.set(self, i, 0);
      if 0 != value_frequencies.get(self, i) {
        remaining_items += 1;
        // Maps to the index in the frequency table with at least 1 entry
        self.tmp_huffman_values_to_visit[remaining_items] = i.try_into().unwrap();
      }
    }
    if remaining_items < 2 {
      // 0 or 1 values distinct values in the data(I.E 0 or 1 frequency entries).
      // Short circuit table setup.
      values_in_tree.set(
        self,
        cast!((self.tmp_huffman_values_to_visit[1]) as usize),
        0,
      );
      Ok(self.tmp_huffman_values_to_visit[1].try_into().unwrap())
    } else {
      // `maybe_huff_used_values` starts at index 1 (index 0 appears unused).
      // Setup the `maybe_huff_used_values` to have the item with the smallest frequency at index 1
      let mut idx = (remaining_items / 2).try_into().unwrap();
      eprintln!("item_counter: {}; idx: {}", remaining_items, idx);
      // Moves the item in `maybe_huff_used_values` with the lowest frequency to the start of the list.
      while idx >= 1 {
        self.move_smallest_value_to_start(idx, value_frequencies, cast!(remaining_items as i16));
        idx -= 1
      }

      // Now build the huffman table
      let mut branch_value;
      loop {
        // Grab the smallest value from the table.
        let smallest_value = self.tmp_huffman_values_to_visit[1];
        if smallest_value < self.tmp_huffman_table_min_node_value {
          values_in_tree.set(self, 0, cast!(smallest_value as u16));
          values_in_tree.shift(self, 1);
        }
        // Make the list shorter by moving the last item into the first item and then shortening the array.
        self.tmp_huffman_values_to_visit[1] = self.tmp_huffman_values_to_visit[remaining_items];
        remaining_items -= 1;

        // Now reshuffle the array to put the smallest item in index 1.
        self.move_smallest_value_to_start(1, value_frequencies, cast!(remaining_items as i16));

        // Grab the next smallest value out of the array.
        let next_smallest_value = self.tmp_huffman_values_to_visit[1];
        if next_smallest_value < self.tmp_huffman_table_min_node_value {
          values_in_tree.set(self, 0, cast!(next_smallest_value as u16));
          values_in_tree.shift(self, 1);
        }

        // Generate a branch node by using the values `max_data_value..`, and storing them in the
        //  frequency and used value table.
        branch_value = next_branch_idx;
        next_branch_idx += 1;
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
        self.tmp_huffman_left_branch_nodes[cast!(branch_value as usize)] =
          cast!(smallest_value as u16);
        self.tmp_huffman_right_branch_nodes[cast!(branch_value as usize)] =
          cast!(next_smallest_value as u16);
        if remaining_items <= 1 {
          break;
        }
      }

      // At this point we have a binary tree stored in `_189` and `_190` with the root node at
      //  `branch_value`. Traversal is by looking up `branch_value` in `_189` and `_190`.

      // Reset the array's offset back to the beginning.
      values_in_tree.set_offset(self, values_in_tree_orig_offset);
      eprintln!("AAAAAAAAAA ::::: AAAAAAAAAA");
      print_tree(
        branch_value,
        self.tmp_huffman_table_min_node_value,
        &self.tmp_huffman_left_branch_nodes,
        &self.tmp_huffman_right_branch_nodes,
      );

      self.calculate_huffman_node_depth(
        cast!(branch_value as i32),
        tree_value_depths,
        values_in_tree,
      );

      values_in_tree.set_offset(self, values_in_tree_orig_offset);
      tree_value_depths.set_offset(self, dat_arr_cursor178_offset);
      self.assign_huffman_encoding(data_values_length, tree_value_depths, values_in_tree);
      eprintln!("{:02X?}", values_in_tree.slice_copy(self));
      Ok(branch_value)
    }
  }
}
