use super::bish_tree::BinaryTreeInvariantError;
use super::table::{LookupTableGenerationError, LookupTables};
use crate::support::LookAheadBitwiseRead;
use std::io;

use crate::level::CompressionLevel;

const EOF_FLAG: u16 = 0x1FE;
const U8_MAX: u16 = 0xFF;
const MIN_RUN_LENGTH: u16 = 3;

#[derive(Debug)]
pub enum ExpandError {
  IOError(io::Error),
  BinaryTreeError(BinaryTreeInvariantError),
  InvariantFailue,
  FileExhausted,
}
impl From<io::Error> for ExpandError {
  fn from(error: io::Error) -> Self {
    ExpandError::IOError(error)
  }
}
impl From<LookupTableGenerationError> for ExpandError {
  fn from(error: LookupTableGenerationError) -> Self {
    match error {
      LookupTableGenerationError::IOError(e) => ExpandError::IOError(e),
      LookupTableGenerationError::BinaryTreeError(e) => ExpandError::BinaryTreeError(e),
      LookupTableGenerationError::InvariantFailue => ExpandError::InvariantFailue,
    }
  }
}
struct ExpandData {
  items_until_next_header: usize,
  table: Option<LookupTables>,
}
impl ExpandData {
  pub fn new() -> Self {
    ExpandData {
      items_until_next_header: 0,
      table: None,
    }
  }
  pub fn next_item(&mut self, reader: &mut impl LookAheadBitwiseRead) -> Result<u16, ExpandError> {
    if self.table.is_none() || self.items_until_next_header == 0 {
      self.items_until_next_header = reader.consume(16)?;
      match &mut self.table {
        Some(t) => t.generate(reader)?,
        None => {
          let mut t = LookupTables::new();
          t.generate(reader)?;
          self.table = Some(t);
        }
      }
    }
    let table = match &self.table {
      Some(t) => t,
      None => unreachable!(),
    };
    if self.items_until_next_header == 0 {
      return Err(ExpandError::FileExhausted);
    }
    self.items_until_next_header -= 1;
    let run_length = table.bit_lookup[reader.look_ahead::<usize>(12)?];
    // run_length <= 0xFF are the uncompressed bits.
    // 0x100 <= run_length < 0x1FE are runs (run_length - 0x100 + 3) bits long.
    // 0x1FE == EOF_FLAG == run_length indicates end of file.
    // 0x1FE < run_length indicates use the table.tree to find the real value; this path is not tested
    if run_length > EOF_FLAG {
      // let _283 = 1usize << 3;
      // let bits = reader.look_ahead(3)?;
      // while (run_length276 >= );
      // //   if (data->bits182 & _283)
      // //     run_length276 = data->dat_arr190[run_length276];
      // //   else
      // //     run_length276 = data->dat_arr189[run_length276];
      // //   _283 >>= 1;
      // }
      unimplemented!("This case is never tested; however exists in the original code. Test data: ");
    }
    reader.consume_bits(table.bit_lookup_len[run_length as usize])?;
    Ok(run_length)
  }
  pub fn run_offset(&self, reader: &mut impl LookAheadBitwiseRead) -> Result<usize, ExpandError> {
    let table = match &self.table {
      Some(t) => t,
      None => unreachable!(),
    };
    let mut run_length = table.run_offset_lookup[reader.look_ahead::<usize>(8)?] as usize;
    // let mut var283 = (1 << 7) as u16;
    // let mut read_offset = 7;
    while run_length >= 15 {
      // run_length = if reader.look_ahead_skip(read_offset, 1)? {
      //   table.tree.right[run_length]
      // } else {
      //   table.tree.left[run_length]
      // } as usize;
      // read_offset += 1;
      panic!();
    }
    if run_length >= 15 {
      unimplemented!("This case is never tested; however it exists in the original code.");
    }
    reader.consume_bits(table.run_offset_lookup_len[run_length])?;
    if run_length == 0 {
      Ok(0)
    } else {
      let tmp = run_length - 1;
      Ok((1 << tmp) + reader.consume::<usize>(tmp)?)
    }
  }
}

pub fn do_expand_level(data: &[u8], level: CompressionLevel) -> Result<Box<[u8]>, ExpandError> {
  use crate::support::LookAheadBitwiseReader;

  let mut reader = LookAheadBitwiseReader::new(&data[..]);
  let mut writer = Vec::with_capacity(level.buffer_size());
  expand(&mut reader, &mut writer, level)?;
  Ok(writer.into_boxed_slice())
}

pub fn expand(
  reader: &mut impl LookAheadBitwiseRead,
  writer: &mut impl io::Write,
  level: CompressionLevel,
) -> Result<(), ExpandError> {
  let mut buffer = vec![0u8; level.buffer_size()];
  let mut has_filled_buffer = false;
  let mut buffer_idx = 0;
  let mut expand_data = ExpandData::new();

  // While we have something to read; or we are expecting more items.
  while !reader.look_ahead_bits(1)?.is_empty() || expand_data.items_until_next_header > 0 {
    let item = expand_data.next_item(reader)?;
    if item == EOF_FLAG {
      break;
    } else if item <= U8_MAX {
      buffer[buffer_idx] = item as u8;
      buffer_idx += 1;
      if buffer_idx >= buffer.len() {
        writer.write_all(&buffer[..buffer_idx])?;
        buffer_idx = 0;
        has_filled_buffer = true;
      }
    } else {
      let run_length = (item - (U8_MAX + 1) + MIN_RUN_LENGTH) as usize;
      let run_offset = expand_data.run_offset(reader)?;
      let run_start = if has_filled_buffer {
        (buffer.len() + buffer_idx) - 1 - run_offset
      } else {
        buffer_idx - 1 - run_offset
      };
      for i in 0..run_length {
        buffer[buffer_idx] = buffer[(run_start + i) % buffer.len()];
        buffer_idx += 1;
        if buffer_idx >= buffer.len() {
          writer.write_all(&buffer[..buffer_idx])?;
          buffer_idx = 0;
          has_filled_buffer = true;
        }
      }
    }
  }
  if buffer_idx > 0 {
    writer.write_all(&buffer[..buffer_idx])?;
  }
  Ok(())
}

#[cfg(test)]
mod tests {
  use super::*;

  #[cfg(test)]
  mod expand_data {
    use super::*;
    use crate::support::lookahead_reader::ExpectedCallLookAheadBitwiseReader;

    #[test]
    fn test_next_item() {
      // Uncompressed data is [0x1A, 0x1A]
      let data: Vec<u8> = vec![0x00, 0x03, 0x20, 0x04, 0x3F, 0xF0, 0x1A, 0xE7, 0xC0, 0x02];
      let mut reader = ExpectedCallLookAheadBitwiseReader::new(
        &data[..],
        &[16, 5, 3, 3, 3, 2, 3, 9, 1, 9, 1, 1, 9, 1, 5, 5, 1, 1, 1],
      );
      let mut data = ExpandData::new();
      assert_eq!(26, data.next_item(&mut reader).unwrap());
      assert_eq!(26, data.next_item(&mut reader).unwrap());
      assert_eq!(510, data.next_item(&mut reader).unwrap());
      assert_eq!(0, data.items_until_next_header);
      // Doesn't actually use the last bit; surprisingly
      assert_eq!(reader.look_ahead_bits(16).unwrap(), vec![false]);
    }

    #[test]
    fn test_next_item_run_length_eq_eof() {
      let data: Vec<u8> = vec![
        0x47, 0xDC, 0x8F, 0xFF, 0x21, 0x01, 0x90, 0x70, 0xA, 0x00, 0xEA, 0x00, 0x00,
      ];
      let mut reader = ExpectedCallLookAheadBitwiseReader::new(
        &data[..],
        &[16, 5, 2, 9, 4, 4, 9, 4, 9, 9, 4, 4, 9, 5, 5],
      );
      let mut data = ExpandData::new();
      let expected_results: Vec<u16> = vec![
        0, 0, 0, 0, 0, 251, 251, 5, 5, 246, 0, 0, 255, 255, 254, 254, 253, 252, 252, 251, 236, 242,
        4, 8, 1, 15, 19, 19, 19, 20, 255, 0, 239, 17, 0, 1, 11, 11, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 245, 245, 236, 237, 237, 237, 241, 243, 243, 0, 255, 0, 0, 1, 8, 10, 9, 17, 17, 17,
        18, 17, 8, 9, 241, 0, 0, 0, 242, 242, 241, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14, 14, 245, 246,
        12, 12, 243, 244, 246, 246, 10, 10, 246, 245, 11, 11, 245, 245, 11, 10, 14, 13, 244, 244,
        246, 245, 15, 15, 15, 241, 241, 241, 11, 10, 14, 11, 244, 245, 243, 244, 12, 13, 9, 10, 9,
        243, 244, 9, 9, 10, 238, 239, 239, 12, 12, 244, 244, 17, 17, 18, 246, 247, 247, 247, 247,
        247, 17, 17, 18, 238, 239, 239, 9, 9, 9, 12, 13, 247, 246, 247, 245, 244, 16, 15, 15, 241,
        241, 240, 12, 11, 11, 12, 245, 242, 244, 245, 11, 12, 12, 12, 243, 242, 12, 13, 244, 244,
        245, 246, 10, 11, 10, 11, 242, 242, 243, 0, 0, 15, 14, 14, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0,
        1, 1, 1, 1, 2, 247, 248, 247, 247, 247, 248, 251, 9, 10, 10, 12, 12, 8, 247, 247, 247, 245,
        245, 245, 17, 17, 16, 17, 245, 246, 245, 12, 12, 13, 238, 239, 238, 239, 11, 11, 11, 245,
        245, 245, 17, 18, 17, 18, 243, 244, 244, 244, 244, 244, 253, 245, 245, 245, 243, 243, 243,
        8, 9, 10, 8, 12, 12, 12, 237, 236, 237, 237, 10, 11, 10, 10, 246, 246, 245, 246, 19, 19,
        20, 19, 244, 244, 244, 12, 12, 12, 5, 10, 10, 10, 10, 244, 243, 244, 252, 252, 0, 4, 252,
        0, 1, 1, 1, 2, 246, 245, 245, 244, 245, 245, 244, 245, 9, 9, 9, 9, 9, 10, 9, 9, 1, 2, 1, 1,
        1, 10, 1, 2, 2, 1, 2, 245, 245, 4, 3, 4, 6, 5, 2, 10, 9, 10, 9, 11, 11, 11, 4, 245, 246,
        245, 245, 245, 245, 246, 245, 11, 10, 11, 11, 244, 245, 244, 12, 11, 12, 9, 10, 10, 9, 243,
        242, 243, 13, 14, 13, 241, 15, 241, 15, 237, 236, 237, 237, 19, 19, 20, 19, 247, 246, 246,
        247, 11, 11, 10, 11, 252, 236, 236, 236, 236, 12, 12, 12, 11, 245, 244, 244, 244, 20, 20,
        20, 20, 9, 1, 2, 2, 1, 2, 243, 244, 244, 243, 13, 12, 12, 13, 254, 255, 254, 254, 255, 254,
        255, 254, 254, 255, 254, 254, 255, 254, 255, 245, 244, 245, 11, 12, 11, 0, 0, 1, 2, 1, 2,
        2, 1, 2, 2, 1, 2, 247, 245, 245, 245, 247, 246, 247, 246, 6, 11, 11, 11, 11, 12, 12, 12,
        12, 255, 254, 254, 254, 255, 254, 2, 1, 2, 2, 2, 2, 243, 244, 244, 244, 247, 246, 246, 247,
        252, 254, 0, 248, 248, 249, 248, 249, 7, 8, 7, 8, 7, 8, 5, 251, 255, 254, 255, 255, 254, 2,
        1, 1, 2, 1, 1, 239, 238, 238, 4, 8, 9, 8, 248, 247, 248, 252, 18, 18, 17, 1, 247, 246, 248,
        9, 9, 9, 10, 238, 238, 238, 239, 253, 253, 255, 243, 244, 243, 244, 252, 245, 245, 246,
        245, 245, 252, 10, 11, 10, 11, 244, 244, 12, 12, 11, 12, 12, 11, 12, 254, 254, 254, 254,
        254, 2, 2, 2, 2, 2, 244, 245, 244, 244, 245, 9, 9, 247, 247, 245, 246, 245, 246, 4, 248,
        253, 253, 252, 253, 252, 4, 3, 4, 3, 3, 3, 3, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 2,
        2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 3, 2, 2, 2, 10, 10, 9, 10, 246, 247, 246, 246, 9,
        247, 254, 2, 9, 247, 254, 254, 254, 253, 254, 254, 254, 254, 255, 254, 254, 254, 254, 255,
        254, 254, 254, 254, 254, 253, 254, 253, 254, 253, 254, 253, 254, 9, 248, 11, 10, 10, 11,
        245, 246, 246, 245, 8, 12, 12, 12, 12, 245, 246, 245, 246, 4, 13, 12, 13, 13, 11, 12, 12,
        12, 254, 255, 254, 255, 254, 255, 1, 2, 1, 2, 1, 2, 244, 244, 244, 245, 9, 9, 9, 9, 239,
        238, 239, 239, 238, 18, 17, 17, 18, 17, 247, 247, 247, 247, 245, 246, 246, 245, 12, 12, 12,
        13, 10, 11, 10, 11, 5, 255, 255, 254, 255, 255, 255, 246, 246, 246, 246, 10, 10, 10, 10, 1,
        1, 1, 2, 1, 1, 251, 7, 246, 10, 249, 245, 246, 245, 246, 246, 247, 246, 248, 11, 12, 12,
        12, 244, 244, 244, 245, 16, 17, 17, 17, 17, 239, 239, 239, 239, 240, 8, 10, 9, 10, 243,
        244, 244, 244, 18, 18, 17, 18, 18, 238, 238, 239, 238, 238, 11, 10, 10, 11, 243, 243, 244,
        243, 252, 10, 11, 10, 11, 10, 10, 10, 10, 239, 238, 238, 239, 238, 18, 17, 18, 18, 17, 246,
        246, 246, 246, 13, 12, 13, 13, 254, 254, 255, 254, 254, 255, 6, 250, 1, 2, 2, 1, 2, 2, 243,
        243, 244, 243, 244, 244, 244, 244, 247, 253, 254, 253, 254, 253, 253, 253, 8, 20, 19, 19,
        19, 19, 237, 237, 237, 237, 236, 11, 11, 10, 11, 11, 10, 11, 11, 10, 2, 2, 2, 2, 2, 2, 254,
        254, 254, 254, 254, 254, 246, 245, 245, 246, 4, 12, 13, 12, 13, 4, 252, 1, 3, 9, 9, 9, 9,
        10, 10, 10, 10, 10, 246, 246, 246, 246, 246, 10, 9, 10, 9, 2, 2, 2, 2, 2, 2, 12, 11, 12,
        11, 11, 11, 12, 11, 11, 249, 244, 245, 244, 11, 11, 245, 245, 244, 244, 245, 244, 245, 16,
        17, 17, 17, 16, 17, 239, 240, 239, 239, 239, 240, 0, 2, 2, 2, 2, 254, 254, 254, 254, 255,
        12, 12, 11, 12, 12, 247, 247, 9, 9, 12, 11, 12, 7, 245, 245, 244, 245, 245, 244, 245, 245,
        244, 245, 11, 12, 11, 11, 12, 10, 10, 11, 9, 243, 242, 243, 243, 10, 10, 10, 10, 250, 244,
        244, 244, 245, 243, 244, 244, 244, 12, 12, 12, 13, 246, 247, 246, 247, 17, 17, 17, 17, 17,
        239, 239, 239, 239, 239, 9, 10, 9, 10, 10, 10, 10, 11, 239, 238, 239, 239, 239, 17, 17, 17,
        18, 17, 245, 246, 246, 246, 11, 12, 12, 12, 7, 3, 2, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 253, 253, 254, 253,
        254, 254, 254, 254, 254, 254, 2, 2, 2, 2, 2, 2, 249, 6, 246, 246, 246, 246, 245, 246, 245,
        245, 20, 19, 19, 19, 19, 237, 237, 237, 237, 236, 11, 11, 10, 11, 243, 243, 244, 243, 252,
        4, 13, 12, 13, 13, 13, 13, 14, 13, 247, 245, 246, 246, 245, 244, 245, 244, 254, 254, 254,
        254, 254, 254, 247, 246, 247, 246, 246, 246, 246, 246, 246, 13, 13, 12, 13, 245, 246, 10,
        11, 12, 12, 12, 12, 236, 237, 236, 236, 237, 19, 20, 20, 19, 20, 244, 244, 244, 244, 10, 9,
        247, 246, 243, 244, 243, 244, 9, 10, 10, 10, 10, 247, 247, 247, 247, 3, 8, 9, 8, 9, 247,
        248, 247, 248, 17, 18, 18, 18, 246, 247, 247, 247, 246, 246, 246, 10, 10, 10, 8, 10, 9, 2,
        1, 2, 2, 1, 2, 254, 255, 254, 254, 255, 254, 255, 255, 248, 249, 248, 8, 8, 0, 2, 4, 250,
        254, 255, 255, 255, 255, 254, 2, 1, 1, 1, 1, 2, 6, 9, 10, 10, 9, 9, 9, 9, 9, 240, 240, 240,
        240, 240, 16, 16, 16, 16, 16, 247, 247, 247, 247, 245, 245, 245, 245, 250, 254, 251, 250,
        251, 251, 252, 251, 19, 18, 252, 251, 252, 251, 5, 4, 5, 4, 238, 237, 5, 4, 5, 5, 252, 253,
        252, 11, 11, 254, 255, 254, 254, 255, 246, 255, 255, 255, 254, 255, 247, 247, 246, 247, 11,
        11, 10, 11, 236, 236, 237, 236, 20, 19, 20, 20, 245, 246, 245, 245, 247, 247, 247, 247, 11,
        12, 11, 11, 13, 13, 12, 1, 255, 0, 255, 255, 255, 255, 1, 1, 1, 1, 0, 244, 243, 243, 12,
        11, 11, 0, 0, 10, 254, 255, 255, 255, 0, 0, 4, 4, 248, 8, 12, 13, 12, 246, 246, 246, 246,
        251, 0, 238, 239, 238, 238, 18, 18, 17, 18, 0, 0, 0, 0, 0, 0, 255, 1, 255, 1, 0, 0, 0, 0,
        0, 244, 244, 244, 248, 246, 247, 248, 254, 254, 255, 255, 255, 1, 1, 1, 2, 0, 1, 1, 1, 1,
        2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 255,
        255, 0, 15, 15, 16, 0, 0, 0, 0, 240, 241, 241, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 255, 255, 254, 255, 255, 254, 255, 255,
        255, 255, 15, 13, 13, 245, 245, 245, 12, 11, 12, 10, 10, 11, 0, 0, 0, 0, 246, 245, 246, 10,
        11, 10, 0, 0, 0, 0, 245, 246, 246, 245, 246, 248, 11, 12, 11, 13, 13, 0, 0, 0, 0, 0, 247,
        248, 247, 9, 8, 9, 0, 241, 241, 241, 241, 15, 15, 15, 15, 0, 0, 0, 0, 243, 243, 246, 246,
        245, 10, 253, 13, 10, 247, 247, 247, 10, 10, 9, 13, 13, 0, 0, 0, 243, 243, 13, 13, 0, 0, 0,
        243, 243, 244, 244, 9, 8, 9, 12, 12, 0, 0, 0, 0, 0, 0, 244, 244, 244, 245, 13, 13, 11, 10,
        0, 0, 0, 245, 246, 10, 11, 0, 0, 0, 246, 245, 244, 245, 12, 12, 245, 245, 11, 11, 10, 11,
        0, 255, 0, 246, 246, 10, 10, 0, 1, 0, 245, 246, 244, 244, 15, 15, 15, 241, 241, 241, 12,
        12, 245, 245, 11, 11, 10, 10, 0, 0, 0, 246, 246, 10, 10, 0, 0, 0, 246, 246, 244, 244, 15,
        14, 15, 241, 242, 241, 11, 12, 243, 243, 15, 16, 15, 245, 244, 12, 11, 241, 240, 241, 11,
        12, 247, 248, 247, 17, 16, 17, 239, 240, 239, 12, 12, 247, 246, 246, 18, 18, 19, 255, 243,
        242, 14, 13, 5, 10, 9, 10, 246, 247, 246, 251, 0, 0, 0, 0, 8, 248, 0, 0, 0, 0, 1, 237, 238,
        238, 9, 9, 9, 8, 8, 8, 238, 238, 238, 18, 18, 18, 248, 248, 248, 246, 243, 3, 246, 11, 10,
        10, 245, 244, 245, 8, 10, 11, 244, 245, 244, 17, 16, 16, 17, 0, 0, 239, 240, 240, 239, 11,
        11, 11, 11, 10, 11, 0, 0, 1, 0, 0, 255, 0, 0, 245, 246, 245, 11, 11, 11, 3, 12, 12, 12, 11,
        10, 11, 3, 1, 1, 1, 255, 255, 255, 254, 238, 240, 239, 239, 11, 11, 11, 11, 12, 12, 255,
        255, 255, 255, 255, 255, 255, 254, 255, 255, 255, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 244,
        244, 245, 245, 245, 245, 20, 20, 20, 236, 236, 236, 11, 11, 11, 9, 9, 9, 248, 244, 244, 12,
        12, 253, 10, 246, 247, 246, 10, 9, 10, 237, 238, 238, 18, 18, 19, 246, 3, 244, 244, 246,
        246, 246, 10, 10, 10, 246, 246, 247, 5, 8, 9, 9, 9, 8, 9, 254, 255, 255, 255, 255, 0, 0,
        255, 255, 0, 255, 255, 255, 255, 0, 255, 0, 0, 0, 15, 247, 248, 239, 238, 239, 239, 239,
        247, 246, 248, 0, 13, 13, 255, 248, 251, 5, 252, 14, 20, 2, 17, 16, 16, 240, 240, 239, 254,
        5, 4, 4, 3, 2, 2, 1, 1, 0, 0, 10, 0, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 17, 9, 247, 239, 251,
        251, 252, 252, 253, 254, 254, 255, 255, 0, 0, 246, 0, 0, 255, 255, 254, 254, 253, 252, 252,
        251, 236, 242, 4, 8, 1, 243, 243, 0, 8, 10, 9, 17, 17, 17, 18, 17, 8, 9, 241, 0, 0, 0, 1,
        0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 2, 247, 248, 247, 246, 247, 246, 251, 243, 242,
        246, 243, 3, 246, 11, 10, 10, 245, 244, 245, 8, 10, 11, 244, 245, 244, 11, 11, 11, 243,
        243, 243, 18, 18, 17, 18, 0, 5, 10, 10, 10, 10, 13, 14, 13, 247, 246, 246, 247, 11, 11, 10,
        11, 252, 245, 245, 245, 247, 246, 247, 246, 254, 2, 1, 1, 1, 1, 2, 6, 252, 254, 0, 2, 1, 1,
        2, 1, 1, 1, 247, 246, 248, 247, 248, 247, 248, 253, 9, 9, 9, 9, 246, 246, 246, 246, 246,
        13, 13, 12, 13, 10, 9, 0, 194, 194, 194, 194, 16, 12, 7, 7, 9, 9, 9, 10, 9, 13, 13, 14, 13,
        13, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 9, 10, 9, 10,
        0, 0, 0, 1, 0, 12, 14, 15, 253, 254, 253, 253, 254, 253, 253, 243, 243, 243, 243, 243, 243,
        247, 247, 247, 247, 247, 242, 243, 243, 242, 243, 243, 242, 243, 243, 242, 243, 243, 246,
        247, 247, 247, 247, 242, 243, 243, 242, 243, 243, 253, 253, 253, 253, 253, 253, 253, 13,
        13, 17, 0, 255, 0, 0, 36, 0, 0, 0, 0, 36, 0, 0, 0, 0, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, 0,
        0, 0, 0, 0, 0, 0, 0, 110,
      ];

      for (i, exp) in expected_results.iter().enumerate() {
        assert_eq!(*exp, data.next_item(&mut reader).unwrap(), "Index {}", i);
      }
      assert_eq!(
        reader.look_ahead_bits(16).unwrap(),
        vec![false, false, false, false, false]
      );
    }
  }

  #[test]
  fn test_expand() {
    let data: Vec<u8> = vec![0x00, 0x03, 0x20, 0x04, 0x3F, 0xF0, 0x1A, 0xE7, 0xC0, 0x02];
    assert_eq!(
      [0x1A, 0x1A],
      do_expand_level(&data, CompressionLevel::Level0).unwrap()[..]
    );
  }
}
