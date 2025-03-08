use std::convert::TryInto;
use std::io::Read;

use crate::compress::RCompressData;
use crate::consts::{MAX_RUN_LENGTH140, MIN_RUN_LENGTH135_IS_3};
use crate::support::BitwiseWrite;

// This file is responsible for creating a "Hash Chain". 
// Possbily an implementation of Rabin-Karp algorithm
impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn199(&mut self, uncompressed_buffer_index200: i16, var201: i16) {
    let (dat168, dat169) = pure_fn199(
      &self.dat_arr163,
      &self.uncompressed_buffer,
      self.max_uncompressed_data_size,
      cast!(uncompressed_buffer_index200 as usize),
      cast!(var201 as usize),
    );
    self.dat168 = cast!(dat168 as i16);
    if let Some(val) = dat169 {
      self.dat169 = cast!(val as i16);
    }
  }
}
const MAX_RUN_COPY_CHECK_ATTEMPTS: usize = 128;
fn pure_fn199(
  dat_arr163: &[i16],
  uncompressed_buffer: &[u8],
  max_data_size: usize,
  start_index: usize,
  test_index_start: usize,
) -> (usize, Option<usize>) {
  let mut largest_run = 0;
  let mut largest_run_offset: Option<usize> = None;
  let mut test_index = test_index_start;
  for _ in 0..MAX_RUN_COPY_CHECK_ATTEMPTS {
    if dat_arr163[test_index] < 0 {
      break;
    }
    test_index = dat_arr163[test_index].try_into().unwrap();
    let mut run_length = 0;
    while run_length < MAX_RUN_LENGTH140 {
      if uncompressed_buffer[start_index + run_length]
        != uncompressed_buffer[test_index + run_length]
      {
        break;
      }
      run_length += 1;
    }
    if run_length < MIN_RUN_LENGTH135_IS_3 {
      // continue;
    } else if run_length > largest_run {
      let offset;
      if start_index < (test_index + 1) {
        offset = max_data_size + start_index - 1 - test_index
      } else {
        offset = start_index - test_index - 1
      }
      if offset >= max_data_size {
        break;
      } else {
        largest_run = run_length;
        largest_run_offset = Some(offset);
        if run_length >= MAX_RUN_LENGTH140 {
          break;
        }
      }
    }
  }
  (largest_run, largest_run_offset)
}
