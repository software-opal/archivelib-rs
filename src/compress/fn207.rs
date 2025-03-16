use std::io::Read;

use crate::compress::base::{CompressU8ArrayAlias, CompressU16ArrayAlias};
use crate::compress::{CompressError, RCompressData, Result};
use crate::consts::{
  CONST_N141_IS_511, CONST_N142_IS_15, CONST_N143_IS_9, CONST_N145_IS_19, CONST_N147_IS_5,
  CONST_N540_IS_5,
};
use crate::support::BitwiseWrite;
const CHAR_BIT: usize = 8;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  /// Flush buffer?
  ///
  /// ZLib: `_tr_flush_block` - maybe
  /// Obfuscated name: `void _207()`
  pub fn fn207_maybe_flush_state_to_output(&mut self) -> Result<()> {
    let mut var456: u32 = 0_u32;
    let mut var217 = [0; 2 * CONST_N145_IS_19 - 1];

    let mut run_length_root_node = self.build_huffman_encoding(
      cast!(CONST_N141_IS_511 as i32),
      &mut CompressU16ArrayAlias::ByteRunLengthFrequency(0),
      &mut CompressU8ArrayAlias::ByteRunLengthHuffBitLength(0),
      &mut CompressU16ArrayAlias::ByteRunLengthHuffEncoding(0),
    )?;

    // Huffman tables have the property that the frequency of the root node is the sum of the
    //  frequencies of all the nodes in the tree. This saves us having to count the frequencies
    //  again.
    let frequency_sum = self.byte_run_length_frequency[cast!(run_length_root_node as usize)];
    self.output_store.write_bits(u32::from(frequency_sum), 16)?;

    // `run_length_root_node` represents a single value if it's in the range `0..511`.
    // `run_length_root_node` represents a root node of a huffman tree if it's in the range `511..`.
    if run_length_root_node >= cast!(CONST_N141_IS_511 as u32) {
      // The root node represents a branch, meaning there were 2 or mode nodes in the tree.
      self.build_byte_length_encoding_lengths(&mut var217);
      eprintln!("var217: {:02X?}", var217);
      let bit_length_root_node = self.build_huffman_encoding(
        cast!(CONST_N145_IS_19 as i32),
        &mut CompressU16ArrayAlias::Custom(0, &mut var217),
        &mut CompressU8ArrayAlias::BitLengthHuffBitLength(0),
        &mut CompressU16ArrayAlias::BitLengthHuffmanEncoding(0),
      )?;
      if bit_length_root_node >= cast!(CONST_N145_IS_19 as u32) {
        // There are 2 or more nodes in the bit length encoding.
        self.write_bit_length_table_data_to_output(
          cast!(CONST_N145_IS_19 as i16),
          cast!(CONST_N147_IS_5 as i16),
          3_i16,
        )?;
      } else {
        // 1 node in the bit length encoding.
        self
          .output_store
          .write_bits(0_u8, cast!(CONST_N147_IS_5 as usize))?;
        self.output_store.write_bits(
          cast!(bit_length_root_node as u32),
          cast!(CONST_N147_IS_5 as usize),
        )?;
      }
      eprintln!("Starting 222");
      self.fn222()?;
    } else {
      // Byte/Run length root node represents a value(I.E. there is only 1 node).
      self
        .output_store
        .write_bits(0_u8, cast!(CONST_N147_IS_5 as usize))?;
      self
        .output_store
        .write_bits(0_u8, cast!(CONST_N147_IS_5 as usize))?;
      self
        .output_store
        .write_bits(0_u8, cast!(CONST_N143_IS_9 as usize))?;
      self.output_store.write_bits(
        cast!(run_length_root_node as u32),
        cast!(CONST_N143_IS_9 as usize),
      )?;
    }
    run_length_root_node = self.build_huffman_encoding(
      cast!(CONST_N142_IS_15 as i32),
      &mut CompressU16ArrayAlias::RunOffsetBitCountFrequency(0),
      &mut CompressU8ArrayAlias::BitLengthHuffBitLength(0),
      &mut CompressU16ArrayAlias::BitLengthHuffmanEncoding(0),
    )?;
    if run_length_root_node >= cast!(CONST_N142_IS_15 as u32) {
      self.write_bit_length_table_data_to_output(
        cast!(CONST_N142_IS_15 as i16),
        cast!(CONST_N540_IS_5 as i16),
        -1_i16,
      )?;
    } else {
      self
        .output_store
        .write_bits(0_u8, cast!(CONST_N540_IS_5 as usize))?;
      self.output_store.write_bits(
        cast!(run_length_root_node as u32),
        cast!(CONST_N540_IS_5 as usize),
      )?;
    }
    let mut var454 = 0_u32;
    for run_start226 in 0..frequency_sum {
      if run_start226 % 8 == 0 {
        var456 = u32::from(self.byte_run_length_buffer[cast!(var454 as usize)]);
        var454 += 1;
      } else {
        var456 <<= 1;
      }
      if 0 == (var456 & (1 << (CHAR_BIT - 1))) {
        let a1 = i16::from(self.byte_run_length_buffer[cast!(var454 as usize)]);
        self.write_stored_bits_to_buffer(a1)?;
        var454 += 1;
      } else {
        let val = self.byte_run_length_buffer[cast!(var454 as usize)];
        self.write_stored_bits_to_buffer(i16::from(val).wrapping_add(1 << CHAR_BIT))?;
        var454 += 1;
        let var289 = u32::from(self.byte_run_length_buffer[cast!(var454 as usize)])
          + (u32::from(self.byte_run_length_buffer[(var454 + 1) as usize]) << 8);
        var454 += 2;
        self.fn224(cast!(var289 as u16))?;
      }
      if self.uncompressible {
        return Err(CompressError::InputUncompressable);
      }
    }
    for i in 0..CONST_N141_IS_511 {
      self.byte_run_length_frequency[i] = 0_u16;
    }
    for i in 0..CONST_N142_IS_15 {
      self.run_offset_bit_count_frequency[i] = 0_u16;
    }
    Ok(())
  }
}
