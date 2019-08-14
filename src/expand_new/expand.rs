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
    // println!("Gimme some sweet sweet next_item");
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
      // panic!();
    }
    let table = match &self.table {
      Some(t) => t,
      None => unreachable!(),
    };
    if self.items_until_next_header == 0 {
      return Err(ExpandError::FileExhausted);
    }
    self.items_until_next_header -= 1;
    let mut run_length = table.bit_lookup[reader.look_ahead::<usize>(12)?];
    // run_length <= 0xFF are the uncompressed bits.
    // 0x100 <= run_length < 0x1FE are runs (run_length - 0x100 + 3) bits long.
    // 0x1FE == EOF_FLAG == run_length indicates end of file.
    // 0x1FE < run_length indicates use the table.tree to find the real value; this path is not tested
    if run_length > EOF_FLAG {
      // Original - https://github.com/software-opal/archivelib-rs/blob/a8107bfc/archivelib-sys2/c-lib/src/r_expand/get_next_item.c#L30
      let mut skip = 12;
      loop {
        if skip < 16 && reader.look_ahead_skip(skip, 1)? {
          run_length = table.tree.right[run_length as usize];
        } else {
          run_length = table.tree.left[run_length as usize];
        }
        skip += 1;
        if run_length <= EOF_FLAG {
          break;
        }
      }
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
      pending_test!();
    }
    if run_length >= 15 {
      std::process::abort();
      pending_test!("This case is never tested; however it exists in the original code.");
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
      let test_data: Vec<u8> = vec![
        0x47, 0xDC, 0x8C, 0xBB, 0xD1, 0x6C, 0xE3, 0x95, 0xFF, 0xFD, 0xB2, 0xC0, 0x0B, 0xBB, 0xB0,
        0x00, 0x0F, 0xBE, 0xFF, 0xF4, 0x92, 0x49, 0x24, 0x92, 0xCC, 0x78, 0xDE, 0xBD, 0xDD, 0xC6,
        0xEA, 0xBA, 0xF7, 0xBD, 0xE1, 0x76, 0x63, 0x6F, 0x37, 0x83, 0x2A, 0x78, 0x03, 0xEF, 0x0A,
        0x54, 0x93, 0xC0, 0xE4, 0xF0, 0x0B, 0x31, 0xF0, 0x37, 0x5E, 0xB9, 0xC0, 0x95, 0x52, 0x53,
        0xDA, 0xD7, 0xB8, 0xDE, 0x66, 0x66, 0x24, 0x92, 0x49, 0x25, 0xFA, 0xFC, 0xF8, 0x00, 0x02,
        0xEE, 0xEC, 0x02, 0xEC, 0x3C, 0x75, 0x40, 0x00, 0x00,
      ];
      let mut reader = ExpectedCallLookAheadBitwiseReader::new(
        &test_data[..],
        &[
          16, 5, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 9, 6, 4, 2, 4, 2, 2, 2, 2, 2, 4, 4,
          4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 6, 6, 6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 3, 4,
          3, 3, 4, 3, 4, 4, 4, 4, 3, 3, 4, 4, 5, 3, 4, 5, 5, 5, 4, 5, 3, 4, 3, 3, 3, 4, 4, 5, 4, 3,
          4, 3, 6, 9, 5, 5, 4, 3, 4, 3, 3, 3, 6, 9, 3, 6, 9, 3, 4, 3, 5, 4, 4, 4, 3, 4, 3, 3, 5, 4,
          3, 4, 4, 3, 3, 4, 3, 4, 4, 4, 3, 4, 4, 3, 3, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
          6, 3, 6, 2, 6, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 2, 2, 2, 2, 4, 4, 2, 2, 6, 9, 3, 5, 5,
          4,
        ],
      );
      let mut data = ExpandData::new();
      // Setup ExpandData with the data from test::match_sys::paris1_hus::x_coords
      assert_eq!(0, data.next_item(&mut reader).unwrap());
      assert_eq!(
        reader.look_ahead_bits(16).unwrap(),
        vec![false, false, false, false, false, false,]
      );

      // Clear out that data; and use the state from a prior run
      let test_data: Vec<u8> = vec![0x01, 0xFF, 0xB8, 0x00];
      let mut reader = ExpectedCallLookAheadBitwiseReader::new(&test_data[..], &[7, 14]);
      // Setup initial state
      reader.consume_bits(7).unwrap();
      assert_eq!(
        0xFF_DCu16,
        reader.look_ahead(16).unwrap(),
        "bytes: {:#b}",
        reader.look_ahead::<u16>(16).unwrap()
      );
      assert_eq!(110, data.next_item(&mut reader).unwrap());
      assert_eq!(reader.look_ahead_bits(16).unwrap(), vec![false; 11]);
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
