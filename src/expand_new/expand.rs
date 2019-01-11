use super::bish_tree::BinaryTreeInvariantError;
use super::table::{LookupTableGenerationError, LookupTables};
use crate::support::LookAheadBitwiseRead;
use std::io;

use crate::level::CompressionLevel;

const EOF_FLAG: u16 = 0x1FE;
const U8_MAX: u16 = 0xFF;
const MIN_RUN_LENGTH: u16 = 3;
const MAX_RUN_LENGTH: u16 = EOF_FLAG - 1 - (U8_MAX + 1);

#[derive(Debug)]
pub enum ExpandError {
  IOError(io::Error),
  BinaryTreeError(BinaryTreeInvariantError),
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
    self.items_until_next_header -= 1;
    let run_length = table.bit_lookup[reader.look_ahead::<usize>(12)?];
    // run_length <= 0xFF are the uncompressed bits.
    // 0x100 <= run_length < 0x1FE are runs (run_length - 0x100 + 3) bits long.
    // 0x1FE == EOF_FLAG == run_length indicates end of file.
    // 0x1FE < run_length indicates use the table.tree to find the real value; this path is not tested
    if run_length > EOF_FLAG {
      unimplemented!("This case is never tested; however exists in the original code.");
    }
    reader.consume_bits(table.bit_lookup_len[run_length as usize])?;
    Ok(run_length)
  }
  pub fn run_offset(&self, reader: &mut impl LookAheadBitwiseRead) -> Result<usize, ExpandError> {
    let table = match &self.table {
      Some(t) => t,
      None => unreachable!(),
    };
    let run_length = table.run_offset_lookup[reader.look_ahead::<usize>(8)?] as usize;
    if run_length >= 15 {
      unimplemented!("This case is never tested; however it exists in the original code.");
    }
    reader.consume_bits(table.run_offset_lookup_len[run_length])?;
    Ok(run_length)
  }
}

pub fn expand(
  reader: &mut impl LookAheadBitwiseRead,
  writer: &mut impl io::Write,
  level: CompressionLevel,
) -> Result<(), ExpandError> {
  let mut buffer = vec![0u8; level.buffer_size()];
  let mut buffer_idx = 0;
  let mut expand_data = ExpandData::new();

  // While we have something to read; or we are expecting more items.
  while reader.look_ahead_bits(1)?.len() == 1 || expand_data.items_until_next_header > 0 {
    let item = expand_data.next_item(reader)?;
    if item == EOF_FLAG {
      break;
    } else if item <= U8_MAX {
      buffer[buffer_idx] = item as u8;
      buffer_idx += 1;
      if buffer_idx >= buffer.len() {
        writer.write_all(&buffer[..buffer_idx])?;
        buffer_idx = 0;
      }
    } else {
      let run_length = (item - (U8_MAX + 1) + MIN_RUN_LENGTH) as usize;
      let run_offset = expand_data.run_offset(reader)?;
      let run_start = buffer_idx - 1 - run_offset;
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
