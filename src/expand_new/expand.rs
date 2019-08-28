use std::convert::{TryFrom, TryInto};

use std::io;

use super::table::{LookupTableGenerationError, LookupTables};
pub use crate::errors::{BinaryTreeInvariantError, DecompressError};
use crate::level::CompressionLevel;
use crate::support::CorrectLookAheadBitwiseRead;

const EOF_FLAG: u16 = 0x1FE;
const U8_MAX: u16 = 0xFF;
const MIN_RUN_LENGTH: u16 = 3;

impl From<LookupTableGenerationError> for DecompressError {
  fn from(error: LookupTableGenerationError) -> Self {
    match error {
      LookupTableGenerationError::IOError(e) => Self::IOError(e),
      LookupTableGenerationError::BinaryTreeError(e) => Self::BinaryTreeError(e),
      LookupTableGenerationError::InvariantFailure => Self::InvariantFailure,
    }
  }
}
struct ExpandData {
  items_until_next_header: usize,
  table: Option<LookupTables>,
}
impl ExpandData {
  pub fn new() -> Self {
    Self {
      items_until_next_header: 0,
      table: None,
    }
  }
  pub fn next_item(
    &mut self,
    reader: &mut impl CorrectLookAheadBitwiseRead,
  ) -> Result<u16, DecompressError> {
    // println!("Gimme some sweet sweet next_item");
    let table = if self.table.is_none() || self.items_until_next_header == 0 {
      self.items_until_next_header = reader.consume(16)?;
      println!("New table! {:?}", self.items_until_next_header);
      let table = self.table.get_or_insert_with(LookupTables::new);
      table.generate(reader)?;
      table
    } else {
      self.table.as_ref().unwrap()
    };
    if self.items_until_next_header == 0 {
      // Replicate the undefined behavior from the C version
      self.items_until_next_header = u16::max_value().try_into().unwrap();
    } else {
      self.items_until_next_header -= 1;
    }
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
          run_length = table.tree.right[cast!(run_length as usize)];
        } else {
          run_length = table.tree.left[cast!(run_length as usize)];
        }
        skip += 1;
        if run_length <= EOF_FLAG {
          break;
        }
      }
    }
    reader.consume_bits(table.bit_lookup_len[cast!(run_length as usize)])?;
    Ok(run_length)
  }
  pub fn run_offset(
    &self,
    reader: &mut impl CorrectLookAheadBitwiseRead,
  ) -> Result<usize, DecompressError> {
    let table = match &self.table {
      Some(t) => t,
      None => unreachable!(),
    };
    let mut run_length = cast!((table.run_offset_lookup[reader.look_ahead::<usize>(8)?]) as usize);
    // let mut var283 = (1 << 7) as u16;
    let mut read_offset = 7;
    while run_length >= 15 {
      run_length = if reader.look_ahead_skip(read_offset, 1)? {
        cast!((table.tree.right[run_length]) as usize)
      } else {
        cast!((table.tree.left[run_length]) as usize)
      };
      read_offset += 1;
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

pub fn expand(
  reader: &mut impl CorrectLookAheadBitwiseRead,
  writer: &mut impl io::Write,
  level: CompressionLevel,
) -> Result<(), DecompressError> {
  let mut buffer = vec![0_u8; level.buffer_size()];
  let mut buffer_idx = 0;
  let mut expand_data = ExpandData::new();

  // While we have something to read; or we are expecting more items.
  while reader.is_al_eof() {
    let item = expand_data.next_item(reader)?;
    if item == EOF_FLAG {
      break;
    } else if item <= U8_MAX {
      buffer[buffer_idx] = item.try_into().unwrap();
      buffer_idx += 1;
      if buffer_idx >= buffer.len() {
        writer.write_all(&buffer[..buffer_idx])?;
        buffer_idx = 0;
      }
    } else {
      let run_length = (item - (U8_MAX + 1) + MIN_RUN_LENGTH) as usize;
      let run_offset = expand_data.run_offset(reader)?;
      assert!(run_offset < buffer.len());
      let run_start = (buffer.len() + buffer_idx) - 1 - run_offset;
      for i in 0..run_length {
        buffer[buffer_idx] = buffer[(run_start + i) % buffer.len()];
        buffer_idx += 1;
        if buffer_idx >= buffer.len() {
          writer.write_all(&buffer[..buffer_idx])?;
          buffer_idx = 0;
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
  use crate::support::LookAheadBitwiseRead;

  #[cfg(test)]
  mod expand_data {
    use super::*;
    use crate::support::ExpectedCallLookAheadBitwiseReader;

    #[test]
    fn test_next_item() {
      // Uncompressed data is [0x1A, 0x1A]
      let data: Vec<u8> = vec![0x00, 0x03, 0x20, 0x04, 0x3F, 0xF0, 0x1A, 0xE7, 0xC0, 0x02];
      let mut reader = ExpectedCallLookAheadBitwiseReader::new_correct(
        &data[..],
        &[16, 5, 3, 3, 3, 2, 3, 9, 1, 9, 1, 1, 9, 1, 5, 5, 1, 1, 1],
      );
      let mut data = ExpandData::new();
      assert_eq!(26, data.next_item(&mut reader).unwrap());
      assert_eq!(26, data.next_item(&mut reader).unwrap());
      assert_eq!(510, data.next_item(&mut reader).unwrap());
      assert_eq!(0, data.items_until_next_header);
      // Doesn't actually use the last bit; surprisingly
      assert_eq!(reader.look_ahead_bits_nopad(16).unwrap(), vec![false]);
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
      let mut reader = ExpectedCallLookAheadBitwiseReader::new_correct(
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
        reader.look_ahead_bits_nopad(16).unwrap(),
        vec![false, false, false, false, false, false,]
      );

      // Clear out that data; and use the state from a prior run
      let test_data: Vec<u8> = vec![0x01, 0xFF, 0xB8, 0x00];
      let mut reader = ExpectedCallLookAheadBitwiseReader::new_correct(&test_data[..], &[7, 14]);
      // Setup initial state
      reader.consume_bits(7).unwrap();
      assert_eq!(
        0xFF_DC_u16,
        reader.look_ahead(16).unwrap(),
        "bytes: {:#b}",
        reader.look_ahead::<u16>(16).unwrap()
      );
      assert_eq!(110, data.next_item(&mut reader).unwrap());
      assert_eq!(reader.look_ahead_bits_nopad(16).unwrap(), vec![false; 11]);
    }
  }
}
