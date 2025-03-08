use std::convert::TryInto;
use std::fmt;

use crate::errors::BinaryTreeInvariantError;

// Was dat_arr189/dat_arr190
pub struct BinaryTree {
  pub left: [u16; 1021],
  pub right: [u16; 1021],
}

impl BinaryTree {
  pub fn new() -> Self {
    Self {
      left: [0; 1021],
      right: [0; 1021],
    }
  }
}

impl fmt::Debug for BinaryTree {
  fn fmt(&self, formatter: &mut fmt::Formatter) -> fmt::Result {
    formatter
      .debug_struct("BinaryTree")
      .field("left", &&self.left[..])
      .field("right", &&self.right[..])
      .finish()
  }
}

struct BinaryTreeLengthLookupTables {
  table1: [usize; 18],
  table2: [usize; 17],
}

impl BinaryTreeLengthLookupTables {
  fn generate(bit_size: usize, bit_lengths: &[usize]) -> Result<Self, BinaryTreeInvariantError> {
    assert!(bit_size <= 16);
    let remaining_bit_size = 16 - bit_size;
    let mut length_occurance = [0; 17];
    let mut lookup_table1 = [0; 18];
    for &len in bit_lengths {
      length_occurance[len.min(16)] += 1;
    }
    for (i, &len_count) in length_occurance.iter().enumerate().skip(1) {
      // Originally a 16-bit unsigned integer with wrapping addition.
      lookup_table1[i + 1] = (lookup_table1[i] + (len_count << (16 - i))) % 0x10000;
    }
    if lookup_table1[17] != 0 {
      return Err(BinaryTreeInvariantError::Type1);
    }
    for v in lookup_table1.iter_mut().skip(1).take(bit_size) {
      *v >>= remaining_bit_size;
    }

    let mut lookup_table2 = [0; 17];
    for (i, v) in lookup_table2.iter_mut().enumerate() {
      *v = if i == 0 {
        0
      } else if i <= bit_size {
        1 << (bit_size - i)
      } else {
        1 << (16 - i)
      };
    }
    Ok(Self {
      table1: lookup_table1,
      table2: lookup_table2,
    })
  }
}

pub fn generate_binary_tree(
  bit_size: usize,
  output: &mut [u16],
  bit_lengths: &[usize],
  tree: &mut BinaryTree,
) -> Result<(), BinaryTreeInvariantError> {
  let mut lookup_tables = BinaryTreeLengthLookupTables::generate(bit_size, bit_lengths)?;
  let mut tree_index = bit_lengths.len();

  for (i, &bit_len) in bit_lengths.iter().enumerate() {
    if bit_len == 0 {
      continue;
    }
    let temp = lookup_tables.table1[bit_len] + lookup_tables.table2[bit_len];
    if bit_len <= bit_size {
      if temp > output.len() {
        return Err(BinaryTreeInvariantError::Type2);
      }
      for v in output
        .iter_mut()
        .take(temp)
        .skip(lookup_tables.table1[bit_len])
      {
        *v = i.try_into().unwrap();
      }
    } else {
      let mut bit_tmp = lookup_tables.table1[bit_len];
      let mut output_is_left: Option<bool> = None;
      let mut output_index = bit_tmp >> (16 - bit_size);
      for _ in 0..(bit_len - bit_size) {
        let mut out_val = match output_is_left {
          None => output[output_index],
          Some(true) => tree.left[output_index],
          Some(false) => tree.right[output_index],
        };
        if out_val == 0 {
          tree.left[tree_index] = 0;
          tree.right[tree_index] = 0;
          out_val = tree_index.try_into().unwrap();
          match output_is_left {
            None => output[output_index] = out_val,
            Some(true) => tree.left[output_index] = out_val,
            Some(false) => tree.right[output_index] = out_val,
          }
          tree_index += 1;
        }
        output_index = cast!(out_val as usize);
        if bit_tmp & (1 << (15 - bit_size)) == 0 {
          output_is_left = Some(true);
        } else {
          output_is_left = Some(false);
        }
        bit_tmp <<= 1;
      }
      match output_is_left {
        None => output[output_index] = i.try_into().unwrap(),
        Some(true) => tree.left[output_index] = i.try_into().unwrap(),
        Some(false) => tree.right[output_index] = i.try_into().unwrap(),
      }
    }
    lookup_tables.table1[bit_len] += lookup_tables.table2[bit_len]
  }

  Ok(())
}

#[cfg(test)]
mod tests {
  use super::*;

  #[test]
  fn test_generate_lookup_tables_from_1st_call() {
    let lookup_tables = BinaryTreeLengthLookupTables::generate(
      8,
      &[2, 3, 9, 0, 0, 9, 8, 6, 5, 4, 1, 7, 0, 0, 0, 0, 0, 0, 0],
    )
    .unwrap();
    assert_eq!(
      lookup_tables.table1,
      [
        0, 0, 128, 192, 224, 240, 248, 252, 254, 65280, 0, 0, 0, 0, 0, 0, 0, 0
      ]
    );

    assert_eq!(
      lookup_tables.table2,
      [0, 128, 64, 32, 16, 8, 4, 2, 1, 128, 64, 32, 16, 8, 4, 2, 1],
    );
  }

  #[test]
  fn test_generate_lookup_tables_from_3rd_call() {
    let lookup_tables = BinaryTreeLengthLookupTables::generate(
      8,
      &[4, 6, 4, 0, 3, 5, 4, 2, 2, 3, 6, 0, 0, 0, 0, 0, 0, 0, 0],
    )
    .unwrap();

    assert_eq!(
      lookup_tables.table1,
      [0, 0, 0, 128, 192, 240, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    );
    assert_eq!(
      lookup_tables.table2,
      [0, 128, 64, 32, 16, 8, 4, 2, 1, 128, 64, 32, 16, 8, 4, 2, 1]
    );
  }

  #[test]
  fn test_generate_lookup_tables_from_2nd_call() {
    let lookup_tables = BinaryTreeLengthLookupTables::generate(
      12,
      &[
        3, 7, 6, 9, 8, 5, 7, 9, 6, 0, 5, 0, 0, 0, 0, 0, 8, 0, 8, 7, 8, 0, 0, 7, 0, 0, 6, 7, 7, 6,
        0, 8, 8, 8, 8, 0, 8, 8, 0, 0, 8, 0, 8, 6, 8, 0, 8, 0, 8, 8, 0, 0, 8, 0, 8, 0, 0, 8, 8, 7,
        0, 8, 0, 6, 7, 5, 0, 0, 6, 0, 0, 0, 0, 8, 8, 0, 0, 8, 0, 0, 0, 7, 0, 0, 7, 0, 0, 0, 8, 0,
        8, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 7, 0, 8, 7, 8, 0, 0, 0, 0, 0, 0, 8, 0,
        0, 0, 8, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 8, 0, 0, 0, 0, 0, 0, 8, 0, 0,
        0, 8, 0, 0, 0, 8, 0, 0, 8, 0, 0, 0, 6, 8, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        8, 0, 6, 0, 0, 0, 0, 8, 0, 0, 8, 0, 7, 0, 7, 0, 8, 7, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 8, 0, 0, 8, 0, 0, 0, 0, 0, 8, 7, 0, 0, 8, 0, 8, 5, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 8, 0, 7, 8, 8, 8, 0, 8, 7, 0, 8, 8, 4, 4, 6, 6, 8, 8, 8, 8, 0, 8, 8, 8, 8, 8, 0,
        0, 0, 8, 8, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        8,
      ],
    )
    .unwrap();
    assert_eq!(
      lookup_tables.table1,
      [
        0, 0, 0, 0, 512, 1024, 1536, 2304, 2944, 4080, 0, 0, 0, 0, 0, 0, 0, 0
      ]
    );
    assert_eq!(
      lookup_tables.table2,
      [
        0, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1, 8, 4, 2, 1
      ]
    );
  }
}
