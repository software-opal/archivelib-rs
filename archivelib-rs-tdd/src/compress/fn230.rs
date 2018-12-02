use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn230(&mut self, bits_to_load219: i32, item209: &[u8], var231: &mut [u16]) {
    // Sibling method is fn258
    // Called with:
    // (CONST_N141_IS_511, dat_arr180, dat_arr192)
    // (CONST_N145_IS_19, dat_arr181, dat_arr194)
    // (CONST_N142_IS_15, dat_arr181, dat_arr194)
    let mut lookup_table288: [u16; 18] = [0; 18];
    for i in 1..=16 {
      lookup_table288[(i + 1)] = ((lookup_table288[i] + self.dat_arr167[i]) << 1) as u16;
    }
    for i in 0..(bits_to_load219 as usize) {
      var231[i] = lookup_table288[item209[i] as usize];
      lookup_table288[item209[i] as usize] += 1;
    }
  }
}
