use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn230(&mut self, mut bits_to_load219: i32, mut item209: *mut u8, mut _231: *mut u16) {
    // Sibling method is fn258
    // Called with:
    // (CONST_N141_IS_511, dat_arr180, dat_arr192)
    // (CONST_N145_IS_19, dat_arr181, dat_arr194)
    // (CONST_N142_IS_15, dat_arr181, dat_arr194)
    let mut run_start226: i32 = 0;
    let mut lookup_table288: [u16; 18] = [0; 18];
    lookup_table288[1] = 0 as u16;
    run_start226 = 1;
    while run_start226 <= 16 {
      lookup_table288[(run_start226 + 1) as usize] = ((lookup_table288[run_start226 as usize]
        + *self.dat_arr167.offset(run_start226 as isize))
        << 1) as u16;
      run_start226 += 1
    }
    run_start226 = 0;
    while run_start226 < bits_to_load219 {
      let fresh0 = lookup_table288[*item209.offset(run_start226 as isize) as usize];
      lookup_table288[*item209.offset(run_start226 as isize) as usize] =
        lookup_table288[*item209.offset(run_start226 as isize) as usize].wrapping_add(1);
      *_231.offset(run_start226 as isize) = fresh0;
      run_start226 += 1
    }
  }
}
