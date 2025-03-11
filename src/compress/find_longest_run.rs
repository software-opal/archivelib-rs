use std::convert::TryInto;
use std::io::Read;

use crate::compress::RCompressData;
use crate::consts::{MAX_RUN_LENGTH, MIN_RUN_LENGTH};
use crate::support::BitwiseWrite;

// This uses the hash chain, which is stored in arr163, to try to find the longest match
impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn find_longest_run(&mut self, start_index: usize, current_byte_run_hash: i16) {
    let (longest_run, longest_run_offset) = find_longest_run_using_byte_run_hash_table(
      &self.byte_run_hash_table,
      &self.uncompressed_buffer,
      self.max_uncompressed_data_size,
      start_index,
      cast!(current_byte_run_hash as usize),
    );
    self.longest_run = cast!(longest_run as i16);
    if let Some(val) = longest_run_offset {
      self.longest_run_offset = cast!(val as i16);
    }
  }
}
const MAX_RUN_COPY_CHECK_ATTEMPTS: usize = 128;
fn find_longest_run_using_byte_run_hash_table(
  byte_run_hash_table: &[i16],
  uncompressed_buffer: &[u8],
  max_data_size: usize,
  start_index: usize,
  hash_last_3_bytes: usize,
) -> (usize, Option<usize>) {
  let mut largest_run = 0;
  let mut largest_run_offset: Option<usize> = None;
  let mut test_index = hash_last_3_bytes;
  for _ in 0..MAX_RUN_COPY_CHECK_ATTEMPTS {
    if byte_run_hash_table[test_index] < 0 {
      break;
    }
    test_index = byte_run_hash_table[test_index].try_into().unwrap();
    let mut run_length = 0;
    while run_length < MAX_RUN_LENGTH {
      if uncompressed_buffer[start_index + run_length]
        != uncompressed_buffer[test_index + run_length]
      {
        break;
      }
      run_length += 1;
    }
    if run_length < MIN_RUN_LENGTH {
      // continue;
    } else if run_length > largest_run {
      let offset;
      if start_index < (test_index + 1) {
        // What is this path doing!?
        // Something to do with lots of data? Maybe worth investigating further later
        offset = max_data_size + start_index - 1 - test_index
      } else {
        offset = start_index - test_index - 1
      }
      if offset >= max_data_size {
        break;
      } else {
        largest_run = run_length;
        largest_run_offset = Some(offset);
        if run_length >= MAX_RUN_LENGTH {
          break;
        }
      }
    }
  }
  (largest_run, largest_run_offset)
}
