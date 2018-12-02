use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn228(&mut self, mut _229: i32) {
    let mut run_start226: i32 = 0;
    let mut _289: i32 = 0;
    let mut _458: u32 = 0;
    run_start226 = 0;
    while run_start226 <= 16 {
      *self.dat_arr167.offset(run_start226 as isize) = 0 as u16;
      run_start226 += 1
    }
    calculate_pointer_depths(
      self.dat_arr189,
      self.dat_arr190,
      self.dat_arr167,
      0 as u16,
      self.dat174,
      _229 as u16,
    );
    _458 = 0 as u32;
    run_start226 = 16;
    while run_start226 > 0 {
      _458 = (_458)
        .wrapping_add(((*self.dat_arr167.offset(run_start226 as isize)) << 16 - run_start226))
        as u32 as u32;
      run_start226 -= 1
    }
    while _458 != 1 << 16 {
      let ref mut fresh0 = *self.dat_arr167.offset(16);
      *fresh0 = (*fresh0).wrapping_sub(1);
      run_start226 = 15;
      while run_start226 > 0 {
        if *self.dat_arr167.offset(run_start226 as isize) != 0 {
          let ref mut fresh1 = *self.dat_arr167.offset(run_start226 as isize);
          *fresh1 = (*fresh1).wrapping_sub(1);
          *self.dat_arr167.offset((run_start226 + 1) as isize) =
            (*self.dat_arr167.offset((run_start226 + 1) as isize) + 2) as u16;
          break;
        } else {
          run_start226 -= 1
        }
      }
      _458 = _458.wrapping_sub(1)
    }
    run_start226 = 16;
    while run_start226 > 0 {
      _289 = *self.dat_arr167.offset(run_start226 as isize) as i32;
      loop {
        _289 -= 1;
        if !(_289 >= 0) {
          break;
        }
        let fresh2 = self.dat_arr_cursor188;
        self.dat_arr_cursor188 = self.dat_arr_cursor188.offset(1);
        *self.dat_arr_cursor178.offset(*fresh2 as isize) = run_start226 as u8
      }
      run_start226 -= 1
    }
  }
}
