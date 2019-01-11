#[derive(Debug)]
pub enum BinaryTreeInvariantError {
  Type1,
  Type2,
}

pub struct BinaryTree {
  pub left: Vec<u16>,
  pub right: Vec<u16>,
}

impl BinaryTree {
  pub fn new(size: usize) -> Self {
    BinaryTree {
      left: vec![0; size],
      right: vec![0; size],
    }
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
      lookup_table1[i + 1] = lookup_table1[i] + (len_count << (16 - i));
    }
    if lookup_table1[17] % 0x10000 != 0 {
      return Err(BinaryTreeInvariantError::Type1);
    }
    for v in lookup_table1.iter_mut() {
      *v = (*v % 0x10000) >> remaining_bit_size;
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
        *v = i as u16;
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
          out_val = tree_index as u16;
          match output_is_left {
            None => output[output_index] = out_val,
            Some(true) => tree.left[output_index] = out_val,
            Some(false) => tree.right[output_index] = out_val,
          }
          tree_index += 1;
        }
        output_index = out_val as usize;
        if bit_tmp & (1 << (15 - bit_size)) == 0 {
          output_is_left = Some(true);
        } else {
          output_is_left = Some(false);
        }
        bit_tmp <<= 1;
      }
      match output_is_left {
        None => output[output_index] = i as u16,
        Some(true) => tree.left[output_index] = i as u16,
        Some(false) => tree.right[output_index] = i as u16,
      }
    }
    lookup_tables.table1[bit_len] += lookup_tables.table2[bit_len]
  }

  Ok(())
}
