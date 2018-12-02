use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

pub fn calculate_pointer_depths(
  mut left_array_ptr: &[u16],
  mut right_array_ptr: &[u16],
  mut depth_store_ptr: &mut [u16],
  mut depth: usize,
  mut series_start: usize,
  mut curr_idx: usize,
) {
  /*
  * Pointer depth calculation?

  * `left_array_ptr` & `right_array_ptr` contain a series(from `series_start`
  to `curr_idx`) of integers that are `< curr_idx`. If they are between
  `series_start` and `curr_idx`, then it's a pointer to another array index.
  Otherwise it's not. This function calculates the number of non-pointer values
  at each depth by following the pointers until a non-pointer, then
  incrementing the count of depth by 1.

  * Note that the pointers will link to the index of both arrays, and need to
  be explored in both arrays. Each value is unique and there are no loops.

  * Does `left_array_ptr` and `right_array_ptr` represent a binary tree?
  */
  if curr_idx < series_start {
    if depth < 16 {
      depth_store_ptr[depth] += 1;
    } else {
      depth_store_ptr[16] += 1;
    }
  } else {
    calculate_pointer_depths(
      left_array_ptr,
      right_array_ptr,
      depth_store_ptr,
      depth + 1,
      series_start,
      left_array_ptr[curr_idx] as usize,
    );
    calculate_pointer_depths(
      left_array_ptr,
      right_array_ptr,
      depth_store_ptr,
      depth + 1,
      series_start,
      right_array_ptr[curr_idx] as usize,
    );
  };
}

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn228(&mut self, mut var229: i32) {
    for i in 0..16 {
      self.dat_arr167[i] = 0;
    }
    calculate_pointer_depths(
      &self.dat_arr189,
      &self.dat_arr190,
      &mut self.dat_arr167,
      0,
      self.dat174 as usize,
      var229 as usize,
    );
    let mut var458: u32 = 0;
    for i in 1..=16 {
      var458 += (self.dat_arr167[i] as u32) << (16 - i);
    }
    while var458 != (1 << 16) {
      self.dat_arr167[16] -= 1;
      let mut run_start226 = 15;
      while run_start226 > 0 {
        if self.dat_arr167[run_start226] != 0 {
          self.dat_arr167[run_start226] -= 1;
          self.dat_arr167[run_start226 + 1] = self.dat_arr167[run_start226 + 1] + 2;
          break;
        } else {
          run_start226 -= 1;
        }
      }
      var458 -= 1;
    }
    let mut run_start226 = 16;
    while run_start226 > 0 {
      let mut var289 = self.dat_arr167[run_start226];
      loop {
        var289 -= 1;
        if !(var289 >= 0) {
          break;
        }
        self.dat_arr_cursor178[self.dat_arr_cursor188[0]] = run_start226 as u8;
        self.dat_arr_cursor188 = self.dat_arr_cursor188[1..];
      }
      run_start226 -= 1
    }
  }
}
