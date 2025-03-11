use std::convert::{TryFrom, TryInto};
use std::io::Read;

use crate::compress::{RCompressData, Result};
use crate::consts::{
  BYTE_RUN_HASH_BITMASK, CONST_N154_IS_4, END_OF_FILE_FLAG, MAX_RUN_LENGTH140_IS_256,
  MIN_RUN_LENGTH135_IS_3,
};
use crate::support::BitwiseWrite;

const UCHAR_MAX: usize = 255;

// ZLib: UPDATE_HASH macro
fn update_byte_run_hash(
  uncompressed_buffer: &[u8],
  buff_position: usize,
  current_byte_run_hash: i16,
) -> i16 {
  ((current_byte_run_hash << CONST_N154_IS_4) ^ i16::from(uncompressed_buffer[buff_position + 2]))
    & cast!(BYTE_RUN_HASH_BITMASK as i16)
}
/// Insert a value into the byte run hash table.
/// Obfusticated name: _447
fn insert_byte_run_hash_entry(
  byte_run_hash_table: &mut [i16],
  buffer_offset_byte_hash: &mut [i16],
  current_buffer_pos: usize,
  current_byte_run_hash: i16,
) {
  let prev_byte_run_pos = byte_run_hash_table[cast!(current_byte_run_hash as usize)];
  if prev_byte_run_pos != -1 {
    buffer_offset_byte_hash[cast!(prev_byte_run_pos as usize)] = cast!(current_buffer_pos as i16);
  }
  buffer_offset_byte_hash[cast!(current_buffer_pos as usize)] = current_byte_run_hash;
  byte_run_hash_table[cast!(current_buffer_pos as usize)] = prev_byte_run_pos;
  byte_run_hash_table[cast!(current_byte_run_hash as usize)] = cast!(current_buffer_pos as i16);
}

/// Removes an entry from the hash table when we have overwritten the byte in the buffer. For example by loading another `buffer_size` bytes.
/// Obfusticated name: _448
fn clear_byte_run_hash_table_entry(
  byte_run_hash_table: &mut [i16],
  buffer_offset_byte_hash: &mut [i16],
  buffer_position: usize,
) {
  let local204 = buffer_offset_byte_hash[buffer_position];
  if local204 != -1 {
    buffer_offset_byte_hash[buffer_position] = -1;
    byte_run_hash_table[cast!(local204 as usize)] = -1;
  }
}

fn read_all(reader: &mut impl Read, target: &mut [u8]) -> Result<usize> {
  let mut idx = 0;
  loop {
    match reader.read(&mut target[idx..]) {
      Ok(0) => return Ok(idx),
      Ok(n) => idx += n,
      Err(ref e) if e.kind() == std::io::ErrorKind::Interrupted => {}
      Err(e) => return Err(e.into()),
    }
  }
}

fn read_one(reader: &mut impl Read) -> Result<Option<u8>> {
  let mut dat = [0];
  match read_all(reader, &mut dat)? {
    0 => Ok(None),
    1 => Ok(Some(dat[0])),
    _ => unreachable!(),
  }
}

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn compress(&mut self) -> Result<()> {
    let mut buffer_pos: usize = 0;
    let size_bitmask280 = self.max_uncompressed_data_size_bitmask;
    let max_size279 = self.max_uncompressed_data_size;
    let mut remaining_data = read_all(
      &mut self.input_store,
      &mut self.uncompressed_buffer[..max_size279],
    )?;
    let mut buffer_end_idx = (remaining_data & size_bitmask280) as usize;

    self.longest_run_offset = 0;
    self.longest_run = 0;
    let mut current_byte_run_hash = cast!(
      (((u16::from(self.uncompressed_buffer[buffer_pos]) << CONST_N154_IS_4)
        ^ u16::from(self.uncompressed_buffer[buffer_pos + 1]))
        & BYTE_RUN_HASH_BITMASK) as i16
    );
    current_byte_run_hash =
      update_byte_run_hash(&self.uncompressed_buffer, buffer_pos, current_byte_run_hash)
        + cast!(max_size279 as i16);

    // Load the first block of data into the longest run functions
    while remaining_data > MAX_RUN_LENGTH140_IS_256 + 4 {
      self.find_longest_run(buffer_pos, current_byte_run_hash);
      if (self.longest_run) < 3 {
        let val = u16::from(self.uncompressed_buffer[buffer_pos]);
        self.fn202(val, 0)?;
        insert_byte_run_hash_entry(
          &mut self.byte_run_hash_table,
          &mut self.buffer_offset_byte_hash,
          buffer_pos,
          current_byte_run_hash,
        );
        buffer_pos += 1;
        current_byte_run_hash =
          update_byte_run_hash(&self.uncompressed_buffer, buffer_pos, current_byte_run_hash)
            + cast!(max_size279 as i16);
        remaining_data -= 1;
      } else {
        remaining_data -= cast!((self.longest_run) as usize);
        let a1 = (self.longest_run + cast!((UCHAR_MAX + 1 - MIN_RUN_LENGTH135_IS_3) as i16))
          .try_into()
          .unwrap();
        let a2 = self.longest_run_offset.try_into().unwrap();
        self.fn202(a1, a2)?;
        loop {
          self.longest_run -= 1;
          if self.longest_run < 0 {
            break;
          }
          insert_byte_run_hash_entry(
            &mut self.byte_run_hash_table,
            &mut self.buffer_offset_byte_hash,
            buffer_pos,
            current_byte_run_hash,
          );
          buffer_pos += 1;
          current_byte_run_hash =
            update_byte_run_hash(&self.uncompressed_buffer, buffer_pos, current_byte_run_hash)
              + cast!(max_size279 as i16)
        }
      }
    }

    while remaining_data < 256 {
      let byte = match read_one(&mut self.input_store)? {
        None => break,
        Some(n) => n,
      };
      self.uncompressed_buffer[buffer_end_idx] = byte;
      if (buffer_end_idx) < 256 - 1 {
        self.uncompressed_buffer[buffer_end_idx + max_size279] = self.uncompressed_buffer[buffer_end_idx]
      }
      clear_byte_run_hash_table_entry(
        &mut self.byte_run_hash_table,
        &mut self.buffer_offset_byte_hash,
        buffer_end_idx,
      );
      buffer_end_idx = (buffer_end_idx + 1) & size_bitmask280;
      remaining_data += 1
    }

    // Now we loop over the remaining data in the buffer; `remaining_data` doesn't represent the
    //  number of remaining bits in the file.
    while remaining_data > 0 {
      self.find_longest_run(buffer_pos, current_byte_run_hash);
      if self.longest_run > cast!(remaining_data as i16) {
        self.longest_run = cast!(remaining_data as i16)
      }
      if (self.longest_run) < 3 {
        self.longest_run = 1;
        let val = u16::from(self.uncompressed_buffer[buffer_pos]);
        self.fn202(val, 0)?;
      } else {
        let a1 =
          cast!((self.longest_run + cast!((UCHAR_MAX + 1 - MIN_RUN_LENGTH135_IS_3) as i16)) as u16);
        let a2 = cast!((self.longest_run_offset) as u16);
        self.fn202(a1, a2)?;
      }
      loop {
        self.longest_run -= 1;
        if self.longest_run < 0 {
          break;
        }
        let byte_or_run_length203 = match read_one(&mut self.input_store)? {
          None => break,
          Some(n) => n,
        };
        self.uncompressed_buffer[buffer_end_idx] = byte_or_run_length203;
        if (buffer_end_idx) < 256 - 1 {
          self.uncompressed_buffer[buffer_end_idx + max_size279] = self.uncompressed_buffer[buffer_end_idx]
        }
        clear_byte_run_hash_table_entry(
          &mut self.byte_run_hash_table,
          &mut self.buffer_offset_byte_hash,
          buffer_end_idx,
        );
        buffer_end_idx = (buffer_end_idx + 1) & size_bitmask280;
        insert_byte_run_hash_entry(
          &mut self.byte_run_hash_table,
          &mut self.buffer_offset_byte_hash,
          buffer_pos,
          current_byte_run_hash,
        );
        buffer_pos = (buffer_pos + 1) & size_bitmask280;
        current_byte_run_hash =
          update_byte_run_hash(&self.uncompressed_buffer, buffer_pos, current_byte_run_hash)
            + cast!(max_size279 as i16)
      }
      loop {
        if self.longest_run < 0 {
          break;
        }
        self.longest_run -= 1;
        insert_byte_run_hash_entry(
          &mut self.byte_run_hash_table,
          &mut self.buffer_offset_byte_hash,
          buffer_pos,
          current_byte_run_hash,
        );
        buffer_pos = (buffer_pos + 1) & size_bitmask280;
        current_byte_run_hash =
          update_byte_run_hash(&self.uncompressed_buffer, buffer_pos, current_byte_run_hash)
            + cast!(max_size279 as i16);
        remaining_data -= 1;
      }
    }
    self.fn202(
      cast!((END_OF_FILE_FLAG + (UCHAR_MAX + 1 - MIN_RUN_LENGTH135_IS_3)) as u16),
      0,
    )?;
    self.finalise_compresson197()
  }
}
