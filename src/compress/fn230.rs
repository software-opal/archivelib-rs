use std::io::Read;

use super::array_alias::ArrayAlias;
use crate::compress::{CompressU8ArrayAlias, CompressU16ArrayAlias, RCompressData};
use crate::support::BitwiseWrite;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  /// Calculates each value's encoding bits based on their depth.
  ///
  /// This uses the value depths and the depth counts set up by `_228` to assign each value a
  ///  huffman encoding.
  ///
  /// Obfuscated name: `void _230(int _219,uchar *_209,ushort *_231)`
  pub fn assign_huffman_encoding(
    &mut self,
    data_values_length: i32,
    tree_value_depths: &CompressU8ArrayAlias<'_>,
    value_huffman_encoding: &mut CompressU16ArrayAlias<'_>,
  ) {
    // Expand sibling method is fn258
    // Called with:
    // (CONST_N141_IS_511, dat_arr180, dat_arr192)
    // (CONST_N145_IS_19, dat_arr181, dat_arr194)
    // (CONST_N142_IS_15, dat_arr181, dat_arr194)
    let tree_value_depths = tree_value_depths.slice_copy(self);
    let result = generate_huffman_encoding(
      cast!(data_values_length as usize),
      &self.huffman_tree_depth_counts,
      &tree_value_depths,
    );
    // Copy the value to encoding mapping into the tree
    for (i, &val) in result.iter().enumerate() {
      value_huffman_encoding.set(self, i, val);
    }
  }
}

fn generate_huffman_encoding(
  data_values_length: usize,
  huffman_tree_depth_counts: &[u16],
  tree_value_depths: &[u8],
) -> Vec<u16> {
  let mut next_bit_for_depth = [0_u16; 18];
  let mut value_to_bits = vec![0_u16; data_values_length];
  for i in 1..=16 {
    next_bit_for_depth[i + 1] = (next_bit_for_depth[i] + huffman_tree_depth_counts[i]) << 1;
  }
  for (value, &value_node_depth) in tree_value_depths
    .iter()
    .take(data_values_length)
    .enumerate()
  {
    // Note: Values where `value_node_depth` is zero (I.E. not in the tree) will be filled with an
    //  incrementing number.
    value_to_bits[value] = next_bit_for_depth[cast!(value_node_depth as usize)];
    next_bit_for_depth[cast!(value_node_depth as usize)] += 1;
  }
  value_to_bits
}

#[cfg(test)]
mod tests {
  use super::*;
  use std::convert::TryInto;

  #[test]
  fn test_using_embroidermodder_hus_stitch_attrs() {
    // This function uses the same data as the ones below, but checks that the data is correctly
    // Placed back into the compress data, as well as the functionality
    let input = [0_u8; 0];
    let mut output = [0_u8; 0];
    let mut cd = RCompressData::new_with_io_writer(&input[..], &mut output[..], 10, true).unwrap();
    // This represents a tree with values `0x02` and `0x07` at level 2, and `0x01`, `0x03`, `0x05`
    //  and `0x06` at level 3. It will produce an encoding where `0x02` is represented by `0b00`, and
    //  `0x07` by `0b01`, and then the three nodes at level three by `0b100` through `0b111`.
    cd.huffman_tree_depth_counts = vec![0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    let mut input_dat_arr181 = [0, 3, 2, 3, 0, 3, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    // The data in here doesn't matter, and is probably a remnant of a previous run.
    let mut input_dat_arr194 = [0; 19];

    // Not that the non-tree values are filled with incrementing numbers(so `0x00` gets 0, `0x04`
    //  gets 1, `0x08` gets 2, etc.)
    let output_dat_arr194 = [0, 4, 0, 5, 1, 6, 7, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12];

    cd.assign_huffman_encoding(
      input_dat_arr181.len().try_into().unwrap(),
      &CompressU8ArrayAlias::Custom(0, &mut input_dat_arr181),
      &mut CompressU16ArrayAlias::Custom(0, &mut input_dat_arr194),
    );

    assert_eq!(input_dat_arr194[0x02], 0b00);
    assert_eq!(input_dat_arr194[0x07], 0b01);
    assert_eq!(input_dat_arr194[0x01], 0b100);
    assert_eq!(input_dat_arr194[0x03], 0b101);
    assert_eq!(input_dat_arr194[0x05], 0b110);
    assert_eq!(input_dat_arr194[0x06], 0b111);

    assert_eq!(input_dat_arr194, output_dat_arr194);
  }
  #[test]
  fn test_0() {
    // Lookup table: [0, 0, 0, 6, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      19,
      &[0, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[0, 3, 2, 0, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [14, 0, 3, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![0, 6, 0, 1, 7, 1, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13],
    );
  }
  #[test]
  fn test_1() {
    // Lookup table: [0, 0, 2, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      15,
      &[0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [12, 1, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(result, vec![0, 0, 1, 2, 3, 4, 5, 2, 6, 7, 3, 8, 9, 10, 11],);
  }
  #[test]
  fn test_2() {
    // Lookup table: [0, 0, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      19,
      &[0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [17, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![
        0, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
      ],
    );
  }
  #[test]
  fn test_3() {
    // Lookup table: [0, 0, 2, 6, 14, 28, 60, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      19,
      &[0, 1, 1, 1, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[0, 6, 5, 0, 0, 0, 6, 6, 1, 2, 5, 6, 3, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [10, 1, 3, 7, 14, 30, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![
        0, 60, 28, 1, 2, 3, 61, 62, 0, 2, 29, 63, 6, 4, 5, 6, 7, 8, 9
      ],
    );
  }
  #[test]
  fn test_4() {
    // Lookup table: [0, 0, 0, 0, 12, 30, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      15,
      &[0, 0, 0, 6, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[5, 3, 3, 5, 4, 3, 3, 3, 4, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [4, 0, 0, 6, 15, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![30, 0, 1, 31, 12, 2, 3, 4, 13, 5, 14, 0, 1, 2, 3],
    );
  }
  #[test]
  fn test_5() {
    // Lookup table: [0, 0, 2, 6, 14, 28, 62, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      19,
      &[0, 1, 1, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[3, 2, 5, 0, 0, 6, 6, 5, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [11, 1, 3, 7, 14, 31, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![
        6, 2, 28, 0, 1, 62, 63, 29, 30, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10
      ],
    );
  }
  #[test]
  fn test_6() {
    // Lookup table: [0, 0, 2, 6, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      15,
      &[0, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[1, 0, 0, 0, 3, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [11, 1, 3, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(result, vec![0, 0, 1, 2, 6, 3, 7, 2, 4, 5, 6, 7, 8, 9, 10],);
  }
  #[test]
  fn test_7() {
    // Lookup table: [0, 0, 0, 4, 12, 28, 62, 126, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      19,
      &[0, 0, 2, 2, 2, 3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[4, 5, 5, 0, 0, 0, 7, 7, 2, 2, 3, 5, 6, 3, 4, 0, 0, 0, 0],
    );
    // Lookup table: [7, 0, 2, 6, 14, 31, 63, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![
        12, 28, 29, 0, 1, 2, 126, 127, 0, 1, 4, 30, 62, 5, 13, 3, 4, 5, 6
      ],
    );
  }
  #[test]
  fn test_8() {
    // Lookup table: [0, 0, 0, 2, 10, 30, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      15,
      &[0, 0, 1, 3, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[5, 3, 4, 4, 4, 4, 5, 4, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [4, 0, 1, 5, 15, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![30, 2, 10, 11, 12, 13, 31, 14, 3, 0, 4, 0, 1, 2, 3],
    );
  }
  #[test]
  fn test_9() {
    // Lookup table: [0, 0, 0, 4, 14, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      19,
      &[0, 0, 2, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[3, 4, 2, 4, 0, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [12, 0, 2, 7, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![4, 14, 0, 15, 0, 5, 1, 6, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11],
    );
  }
  #[test]
  fn test_10() {
    // Lookup table: [0, 0, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      15,
      &[0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [13, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(result, vec![0, 0, 1, 2, 3, 4, 5, 6, 1, 7, 8, 9, 10, 11, 12],);
  }
  #[test]
  fn test_11() {
    // Lookup table: [0, 0, 2, 6, 12, 28, 60, 126, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      19,
      &[0, 1, 1, 0, 2, 2, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[7, 7, 6, 0, 0, 0, 6, 5, 1, 2, 4, 4, 5, 6, 0, 0, 0, 0, 0],
    );
    // Lookup table: [8, 1, 3, 6, 14, 30, 63, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![
        126, 127, 60, 0, 1, 2, 61, 28, 0, 2, 12, 13, 29, 62, 3, 4, 5, 6, 7
      ],
    );
  }
  #[test]
  fn test_12() {
    // Lookup table: [0, 0, 0, 0, 12, 30, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      15,
      &[0, 0, 0, 6, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[5, 5, 3, 4, 4, 3, 3, 4, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [4, 0, 0, 6, 15, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![30, 31, 0, 12, 13, 1, 2, 14, 3, 4, 5, 0, 1, 2, 3],
    );
  }
  #[test]
  fn test_13() {
    // Lookup table: [0, 0, 2, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      19,
      &[0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[2, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [16, 1, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![
        2, 0, 3, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
      ],
    );
  }
  #[test]
  fn test_14() {
    // Lookup table: [0, 0, 2, 4, 14, 28, 62, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      19,
      &[0, 1, 0, 3, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[0, 5, 5, 0, 0, 0, 6, 3, 1, 3, 3, 5, 6, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [10, 1, 2, 7, 14, 31, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![0, 28, 29, 1, 2, 3, 62, 4, 0, 5, 6, 30, 63, 4, 5, 6, 7, 8, 9],
    );
  }
  #[test]
  fn test_15() {
    // Lookup table: [0, 0, 0, 2, 12, 28, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      15,
      &[0, 0, 1, 4, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[5, 2, 3, 4, 5, 5, 5, 4, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [4, 0, 1, 6, 14, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![28, 0, 2, 12, 29, 30, 31, 13, 3, 4, 5, 0, 1, 2, 3],
    );
  }
  #[test]
  fn test_16() {
    // Lookup table: [0, 0, 0, 4, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      19,
      &[0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[0, 3, 2, 3, 0, 3, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [13, 0, 2, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![0, 4, 0, 5, 1, 6, 7, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12],
    );
  }
  #[test]
  fn test_17() {
    // Lookup table: [0, 0, 2, 4, 12, 28, 62, 126, 254, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      19,
      &[0, 1, 0, 2, 2, 3, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0],
      &[4, 7, 5, 0, 0, 0, 8, 8, 1, 3, 5, 5, 4, 3, 6, 0, 0, 0, 0],
    );
    // Lookup table: [7, 1, 2, 6, 14, 31, 63, 127, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![
        12, 126, 28, 0, 1, 2, 254, 255, 0, 4, 29, 30, 13, 5, 62, 3, 4, 5, 6
      ],
    );
  }
  #[test]
  fn test_18() {
    // Lookup table: [0, 0, 0, 4, 12, 30, 62, 126, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      15,
      &[0, 0, 2, 2, 3, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[7, 3, 4, 5, 4, 6, 7, 4, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [4, 0, 2, 6, 15, 31, 63, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![126, 4, 12, 30, 13, 62, 127, 14, 5, 0, 1, 0, 1, 2, 3],
    );
  }
  #[test]
  fn test_19() {
    // Lookup table: [0, 0, 2, 4, 12, 30, 62, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      19,
      &[0, 1, 0, 2, 3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[6, 0, 4, 0, 0, 0, 6, 3, 1, 3, 4, 4, 5, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [10, 1, 2, 6, 15, 31, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![
        62, 0, 12, 1, 2, 3, 63, 4, 0, 5, 13, 14, 30, 4, 5, 6, 7, 8, 9
      ],
    );
  }
  #[test]
  fn test_20() {
    // Lookup table: [0, 0, 0, 0, 14, 30, 62, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    let result = generate_huffman_encoding(
      15,
      &[0, 0, 0, 7, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      &[6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0],
    );
    // Lookup table: [4, 0, 0, 7, 15, 31, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 0, 0]
    assert_eq!(
      result,
      vec![62, 0, 1, 14, 63, 30, 2, 3, 4, 5, 6, 0, 1, 2, 3],
    );
  }
}
