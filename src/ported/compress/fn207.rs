use std::io::Read;

use crate::ported::compress::base::{CompressU8ArrayAlias, CompressU16ArrayAlias};
use crate::ported::compress::{CompressError, RCompressData, Result};
use crate::ported::consts::{
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
    self
      .output_store
      .write_bits(cast!(frequency_sum as u16), 16)?;

    // `run_length_root_node` represents a single value if it's in the range `0..511`.
    // `run_length_root_node` represents a root node of a huffman tree if it's in the range `511..`.
    if run_length_root_node >= cast!(CONST_N141_IS_511 as u32) {
      // The root node represents a branch, meaning there were 2 or mode nodes in the tree.
      self.build_byte_length_encoding_lengths(&mut var217);
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
          .write_bits(0, cast!(CONST_N147_IS_5 as usize))?;
        self.output_store.write_bits(
          cast!(bit_length_root_node as u16),
          cast!(CONST_N147_IS_5 as usize),
        )?;
      }
      self.write_byte_run_huff_tree_to_file()?;
    } else {
      // Byte/Run length root node represents a value(I.E. there is only 1 node).
      self
        .output_store
        .write_bits(0, cast!(CONST_N147_IS_5 as usize))?;
      self
        .output_store
        .write_bits(0, cast!(CONST_N147_IS_5 as usize))?;
      self
        .output_store
        .write_bits(0, cast!(CONST_N143_IS_9 as usize))?;
      self.output_store.write_bits(
        cast!(run_length_root_node as u16),
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
        .write_bits(0, cast!(CONST_N540_IS_5 as usize))?;
      self.output_store.write_bits(
        cast!(run_length_root_node as u16),
        cast!(CONST_N540_IS_5 as usize),
      )?;
    }
    // Now that we've written enough information for the decompressor to rebuild the huffman tables,
    // we can now use them to write out the bytes in the buffer.
    let mut buffer_offset = 0_u32;
    let mut run_length_bit_flag: u32 = 0_u32;
    for run_start226 in 0..frequency_sum {
      if run_start226 % 8 == 0 {
        run_length_bit_flag = u32::from(self.byte_run_length_buffer[cast!(buffer_offset as usize)]);
        buffer_offset += 1;
      } else {
        run_length_bit_flag <<= 1;
      }
      if 0 == (run_length_bit_flag & (1 << (CHAR_BIT - 1))) {
        let a1 = i16::from(self.byte_run_length_buffer[cast!(buffer_offset as usize)]);
        self.write_stored_bits_to_buffer(a1)?;
        buffer_offset += 1;
      } else {
        let val = self.byte_run_length_buffer[cast!(buffer_offset as usize)];
        self.write_stored_bits_to_buffer(i16::from(val).wrapping_add(1 << CHAR_BIT))?;
        buffer_offset += 1;
        let var289 = u32::from(self.byte_run_length_buffer[cast!(buffer_offset as usize)])
          + (u32::from(self.byte_run_length_buffer[(buffer_offset + 1) as usize]) << 8);
        buffer_offset += 2;
        self.write_run_offset_value_to_file(cast!(var289 as u16))?;
      }
      if self.uncompressible {
        return Err(CompressError::InputUncompressable);
      }
    }
    // Now clear the frequency buffers ready for the next round.
    for i in 0..CONST_N141_IS_511 {
      self.byte_run_length_frequency[i] = 0_u16;
    }
    for i in 0..CONST_N142_IS_15 {
      self.run_offset_bit_count_frequency[i] = 0_u16;
    }
    Ok(())
  }

  /// Write the given byte/run length's huffman encoding to the output buffer.
  ///
  /// Obfuscated name: `void _223(short _203)`
  pub fn write_stored_bits_to_buffer(&mut self, byte_or_run_length_value: i16) -> Result<()> {
    /*
    `arg203` appears to be the bits in the file most of the time
    */
    let byte_or_run_length_value: usize = byte_or_run_length_value.try_into().unwrap();
    self.output_store.write_bits(
      self.byte_run_length_huff_encoding[byte_or_run_length_value],
      cast!((self.byte_run_length_huff_bit_length[byte_or_run_length_value]) as usize),
    )?;
    Ok(())
  }
}
