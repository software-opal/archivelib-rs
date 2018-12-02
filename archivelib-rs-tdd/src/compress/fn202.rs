use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn202(&mut self, mut byte_or_run_length203: u16, mut _204: u16) {
    self.bitwise_counter185 = (self.bitwise_counter185 >> 1) as u16;
    if self.bitwise_counter185 == 0 {
      self.bitwise_counter185 = (1 << 8 - 1) as u16;
      if self.array165_counter >= self.dat183_IS_CONST_8162 {
        fn207(data);
        if 0 != self.uncompressible {
          return;
        } else {
          self.array165_counter = 0 as u16
        }
      }
      let fresh0 = self.array165_counter;
      self.array165_counter = self.array165_counter.wrapping_add(1);
      self.array165_tmp_counter186 = fresh0;
      *self
        .dat_arr165
        .offset(self.array165_tmp_counter186 as isize) = 0 as u8
    }
    let fresh1 = self.array165_counter;
    self.array165_counter = self.array165_counter.wrapping_add(1);
    *self.dat_arr165.offset(fresh1 as isize) = byte_or_run_length203 as u8;
    let ref mut fresh2 = *self.dat_arr191.offset(byte_or_run_length203 as isize);
    *fresh2 = (*fresh2).wrapping_add(1);
    if byte_or_run_length203 >= 1 << 8 {
      let ref mut fresh3 = *self
        .dat_arr165
        .offset(self.array165_tmp_counter186 as isize);
      *fresh3 = (*fresh3 | self.bitwise_counter185 as u8) as u8;
      let fresh4 = self.array165_counter;
      self.array165_counter = self.array165_counter.wrapping_add(1);
      *self.dat_arr165.offset(fresh4 as isize) = _204 as u8;
      let fresh5 = self.array165_counter;
      self.array165_counter = self.array165_counter.wrapping_add(1);
      *self.dat_arr165.offset(fresh5 as isize) = (_204 >> 8) as u8;
      byte_or_run_length203 = 0 as u16;
      while 0 != _204 {
        byte_or_run_length203 = byte_or_run_length203.wrapping_add(1);
        _204 = (_204 >> 1) as u16
      }
      let ref mut fresh6 = *self.dat_arr193.offset(byte_or_run_length203 as isize);
      *fresh6 = (*fresh6).wrapping_add(1)
    };
  }
}
