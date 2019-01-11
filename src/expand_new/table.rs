// [0x00, 0x03,
//
// // Bits lookup
// self.dat_arr240
// // consumed bits lookup(dat_arr180[dat_arr240[x]])
// self.dat_arr180
//
// // Run offset lookup
// self.dat_arr241
// // 241 length lookup
// self.dat_arr181
//
// // Binary tree(ish) pair. No test cases. No worries
// self.dat_arr190
// self.dat_arr189
use super::bish_tree::{generate_binary_tree, BinaryTree, BinaryTreeInvariantError};
use crate::support::LookAheadBitwiseRead;
use std::io;

#[derive(Debug)]
pub enum LookupTableGenerationError {
  IOError(io::Error),
  BinaryTreeError(BinaryTreeInvariantError),
}

impl From<io::Error> for LookupTableGenerationError {
  fn from(error: io::Error) -> Self {
    LookupTableGenerationError::IOError(error)
  }
}
impl From<BinaryTreeInvariantError> for LookupTableGenerationError {
  fn from(error: BinaryTreeInvariantError) -> Self {
    LookupTableGenerationError::BinaryTreeError(error)
  }
}

#[allow(dead_code)]
pub struct LookupTables {
  pub bit_lookup: Vec<u16>,
  pub bit_lookup_len: Vec<usize>,
  // Should be usize; but due to type requirements for the tree; we use u16
  pub run_offset_lookup: Vec<u16>,
  pub run_offset_lookup_len: Vec<usize>,
  pub tree: BinaryTree,
}

impl LookupTables {
  pub fn new() -> Self {
    Self {
      bit_lookup: vec![0; 4096],
      bit_lookup_len: vec![0; 511],
      run_offset_lookup: vec![0; 256],
      run_offset_lookup_len: vec![0; 19],
      tree: BinaryTree::new(1021),
    }
  }
  pub fn generate(
    &mut self,
    reader: &mut impl LookAheadBitwiseRead,
  ) -> Result<(), LookupTableGenerationError> {
    self.generate_run_offset_lookup(reader, true)?;
    self.generate_bit_lookup(reader)?;
    self.generate_run_offset_lookup(reader, false)?;
    Ok(())
  }
  pub fn generate_run_offset_lookup(
    &mut self,
    reader: &mut impl LookAheadBitwiseRead,
    do_pad_length: bool,
  ) -> Result<(), LookupTableGenerationError> {
    let bits_to_load: usize = reader.consume(5)?;
    if bits_to_load == 0 {
      let offset_const = reader.consume(5)?;
      for e in self.run_offset_lookup.iter_mut() {
        *e = offset_const;
      }
      for e in self.run_offset_lookup_len.iter_mut() {
        *e = 0;
      }
      Ok(())
    } else {
      let mut i = 0;
      while i < bits_to_load {
        let mut bit_length = reader.consume(3)?;
        if bit_length == 7 {
          while reader.consume(1)? {
            bit_length += 1;
          }
        }
        self.run_offset_lookup_len[i] = bit_length;
        i += 1;
        if do_pad_length && i == 3 {
          let pad_length: usize = reader.consume(2)?;
          for _ in 0..pad_length {
            self.run_offset_lookup_len[i] = 0;
            i += 1;
          }
        }
      }
      while i < self.run_offset_lookup_len.len() {
        self.run_offset_lookup_len[i] = 0;
        i += 1;
      }
      let limit = if do_pad_length { 19 } else { 15 };
      generate_binary_tree(
        8,
        &mut self.run_offset_lookup,
        &self.run_offset_lookup_len,
        &mut self.tree,
      )?;
      Ok(())
    }
  }

  pub fn generate_bit_lookup(
    &mut self,
    reader: &mut impl LookAheadBitwiseRead,
  ) -> Result<(), LookupTableGenerationError> {
    let bits_to_load: usize = reader.consume(9)?;
    if bits_to_load == 0 {
      let offset_const = reader.consume(9)?;
      for e in self.bit_lookup.iter_mut() {
        *e = offset_const;
      }
      for e in self.bit_lookup_len.iter_mut() {
        *e = 0;
      }
      Ok(())
    } else {
      let mut i = 0;
      while i < bits_to_load {
        let mut idx = self.run_offset_lookup[reader.look_ahead::<usize>(8)?];
        if idx >= 19 {
          for skip in 8.. {
            idx = if reader.look_ahead_skip(skip, 1)? {
              self.tree.right[idx as usize]
            } else {
              self.tree.left[idx as usize]
            };
            if idx < 19 {
              break;
            }
          }
        }
        reader.consume_bits(self.run_offset_lookup_len[idx as usize])?;
        if idx <= 2 {
          idx = match idx {
            0 => 1,
            1 => reader.consume::<u16>(4)? + 3,
            2 => reader.consume::<u16>(9)? + 20,
            _ => unreachable!(),
          };
          for _ in 0..idx {
            self.bit_lookup_len[i] = 0;
            i += 1;
          }
        } else {
          self.bit_lookup_len[i] = (idx as usize) - 2;
          i += 1;
        }
      }
      for v in self.bit_lookup_len[i..].iter_mut() {
        *v = 0;
      }
      generate_binary_tree(
        12,
        &mut self.bit_lookup,
        &self.bit_lookup_len,
        &mut self.tree,
      )?;
      Ok(())
    }
  }
}

#[cfg(test)]
mod tests {
  use super::*;
  use crate::support::LookAheadBitwiseReader;

  #[test]
  fn reader_calls() {
    let data: Vec<u8> = vec![0x00, 0x03, 0x20, 0x04, 0x3F, 0xF0, 0x1A, 0xE7, 0xC0, 0x02];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);

    assert_eq!(reader.consume::<u16>(16).unwrap(), 0b0000000000000011);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b0000000000000100);
  }

  #[test]
  fn base_data_seperated_calls() {
    let data: Vec<u8> = vec![0x00, 0x03, 0x20, 0x04, 0x3F, 0xF0, 0x1A, 0xE7, 0xC0, 0x02];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);
    reader.consume_bits(16).unwrap();
    let mut tables = LookupTables::new();
    tables
      .generate_run_offset_lookup(&mut reader, true)
      .unwrap();
    assert_eq!(
      tables.run_offset_lookup_len,
      rvec![0x00 => 2, 0x01 => 2, 0x00 => 15]
    );
    assert_eq!(tables.run_offset_lookup, rvec![0x02 => 128, 0x03 => 128]);
  }

  #[test]
  fn base_data() {
    // Uncompressed data is [0x1A, 0x1A]
    let data: Vec<u8> = vec![0x00, 0x03, 0x20, 0x04, 0x3F, 0xF0, 0x1A, 0xE7, 0xC0, 0x02];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);
    reader.consume_bits(16).unwrap();
    let mut tables = LookupTables::new();
    tables.generate(&mut reader).unwrap();

    // The generate functon should have read 9.5 bytes(76 bits)

    assert_eq!(
      reader.look_ahead_bits(16).unwrap(),
      vec![false, false, true, false]
    );
    assert_eq!(tables.bit_lookup, rvec![0x1A => 2048, 0x1FE => 2048]);
    assert_eq!(
      tables.bit_lookup_len,
      rvec![0x00 => 26, 0x01 => 1, 0x00 => 483, 0x01 => 1]
    );
    assert_eq!(tables.run_offset_lookup, rvec![0x00 => 256]);
    assert_eq!(tables.run_offset_lookup_len, rvec![0x00 => 19]);
  }

}
