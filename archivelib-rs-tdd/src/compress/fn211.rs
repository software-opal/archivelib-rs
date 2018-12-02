use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn211(
    &mut self,
    mut var212: i32,
    mut var213: &mut [u16],
    mut var214: &mut [u8],
    mut var215: &mut [u16],
  ) -> i32 {
    let mut run_start226: i32 = 0;
    let mut run_length276: i32 = 0;
    let mut var289: i32 = 0;
    let mut var292: i32 = 0;
    let mut var227: i16 = 0;
    self.dat174 = var212 as i16;
    self.dat_arr_cursor187 = var213;
    self.dat_arr_cursor178 = var214;
    var292 = self.dat174 as i32;
    var227 = 0 as i16;
    *self.dat_arr177.offset(1) = 0 as i16;
    run_start226 = 0;
    while run_start226 < self.dat174 {
      *self.dat_arr_cursor178.offset(run_start226 as isize) = 0 as u8;
      if 0 != *self.dat_arr_cursor187.offset(run_start226 as isize) {
        var227 += 1;
        *self.dat_arr177.offset(var227 as isize) = run_start226 as i16
      }
      run_start226 += 1
    }
    if (var227) < 2 {
      *var215.offset(*self.dat_arr177.offset(1) as isize) = 0 as u16;
      return *self.dat_arr177.offset(1) as i32;
    } else {
      run_start226 = var227 / 2;
      while run_start226 >= 1 {
        self.fn225(
          run_start226,
          self.dat_arr_cursor187,
          self.dat_arr177,
          var227,
        );
        run_start226 -= 1
      }
      self.dat_arr_cursor188 = var215;
      loop {
        run_start226 = *self.dat_arr177.offset(1) as i32;
        if run_start226 < self.dat174 {
          let fresh0 = self.dat_arr_cursor188;
          self.dat_arr_cursor188 = self.dat_arr_cursor188.offset(1);
          *fresh0 = run_start226 as u16
        }
        let fresh1 = var227;
        var227 = var227 - 1;
        *self.dat_arr177.offset(1) = *self.dat_arr177.offset(fresh1 as isize);
        self.fn225(1, self.dat_arr_cursor187, self.dat_arr177, var227);
        run_length276 = *self.dat_arr177.offset(1) as i32;
        if run_length276 < self.dat174 {
          let fresh2 = self.dat_arr_cursor188;
          self.dat_arr_cursor188 = self.dat_arr_cursor188.offset(1);
          *fresh2 = run_length276 as u16
        }
        let fresh3 = var292;
        var292 = var292 + 1;
        var289 = fresh3;
        *self.dat_arr_cursor187.offset(var289 as isize) =
          (*self.dat_arr_cursor187.offset(run_start226 as isize)
            + *self.dat_arr_cursor187.offset(run_length276 as isize)) as u16;
        *self.dat_arr177.offset(1) = var289 as i16;
        self.fn225(1, self.dat_arr_cursor187, self.dat_arr177, var227);
        *self.dat_arr189.offset(var289 as isize) = run_start226 as u16;
        *self.dat_arr190.offset(var289 as isize) = run_length276 as u16;
        if !(var227 > 1) {
          break;
        }
      }
      self.dat_arr_cursor188 = var215;
      self.fn228(var289);
      self.fn230(var212, var214, var215);
      return var289;
    };
  }
}
