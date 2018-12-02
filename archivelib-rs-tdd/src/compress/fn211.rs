use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn211(
    &mut self,
    mut _212: i32,
    mut _213: *mut u16,
    mut _214: *mut u8,
    mut _215: *mut u16,
  ) -> i32 {
    let mut run_start226: i32 = 0;
    let mut run_length276: i32 = 0;
    let mut _289: i32 = 0;
    let mut _292: i32 = 0;
    let mut _227: i16 = 0;
    self.dat174 = _212 as i16;
    self.dat_arr_cursor187 = _213;
    self.dat_arr_cursor178 = _214;
    _292 = self.dat174 as i32;
    _227 = 0 as i16;
    *self.dat_arr177.offset(1) = 0 as i16;
    run_start226 = 0;
    while run_start226 < self.dat174 {
      *self.dat_arr_cursor178.offset(run_start226 as isize) = 0 as u8;
      if 0 != *self.dat_arr_cursor187.offset(run_start226 as isize) {
        _227 += 1;
        *self.dat_arr177.offset(_227 as isize) = run_start226 as i16
      }
      run_start226 += 1
    }
    if (_227) < 2 {
      *_215.offset(*self.dat_arr177.offset(1) as isize) = 0 as u16;
      return *self.dat_arr177.offset(1) as i32;
    } else {
      run_start226 = _227 / 2;
      while run_start226 >= 1 {
        fn225(
          data,
          run_start226,
          self.dat_arr_cursor187,
          self.dat_arr177,
          _227,
        );
        run_start226 -= 1
      }
      self.dat_arr_cursor188 = _215;
      loop {
        run_start226 = *self.dat_arr177.offset(1) as i32;
        if run_start226 < self.dat174 {
          let fresh0 = self.dat_arr_cursor188;
          self.dat_arr_cursor188 = self.dat_arr_cursor188.offset(1);
          *fresh0 = run_start226 as u16
        }
        let fresh1 = _227;
        _227 = _227 - 1;
        *self.dat_arr177.offset(1) = *self.dat_arr177.offset(fresh1 as isize);
        fn225(data, 1, self.dat_arr_cursor187, self.dat_arr177, _227);
        run_length276 = *self.dat_arr177.offset(1) as i32;
        if run_length276 < self.dat174 {
          let fresh2 = self.dat_arr_cursor188;
          self.dat_arr_cursor188 = self.dat_arr_cursor188.offset(1);
          *fresh2 = run_length276 as u16
        }
        let fresh3 = _292;
        _292 = _292 + 1;
        _289 = fresh3;
        *self.dat_arr_cursor187.offset(_289 as isize) =
          (*self.dat_arr_cursor187.offset(run_start226 as isize)
            + *self.dat_arr_cursor187.offset(run_length276 as isize)) as u16;
        *self.dat_arr177.offset(1) = _289 as i16;
        fn225(data, 1, self.dat_arr_cursor187, self.dat_arr177, _227);
        *self.dat_arr189.offset(_289 as isize) = run_start226 as u16;
        *self.dat_arr190.offset(_289 as isize) = run_length276 as u16;
        if !(_227 > 1) {
          break;
        }
      }
      self.dat_arr_cursor188 = _215;
      fn228(data, _289);
      fn230(data, _212, _214, _215);
      return _289;
    };
  }
}
