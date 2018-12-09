use crate::compress::{CompressU16ArrayAlias, CompressU8ArrayAlias, RCompressData, Result};

use crate::support::ArrayAlias;
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn211(
    &mut self,
    var212: i32,
    dat_arr_cursor187: &mut CompressU16ArrayAlias<'_>,
    dat_arr_cursor178: &mut CompressU8ArrayAlias<'_>,
    dat_arr_cursor188: &mut CompressU16ArrayAlias<'_>,
  ) -> Result<i32> {
    let dat_arr_cursor178_offset = dat_arr_cursor178.offset(self);
    let dat_arr_cursor188_offset = dat_arr_cursor188.offset(self);

    self.dat174 = var212 as i16;
    let mut var292 = var212;
    let mut var227 = 0;
    self.dat_arr177[1] = 0;
    for i in 0..(self.dat174 as usize) {
      dat_arr_cursor178.set(self, i, 0);
      if 0 != dat_arr_cursor187.get(self, i) {
        var227 += 1;
        self.dat_arr177[var227] = i as i16
      }
    }
    if var227 < 2 {
      dat_arr_cursor188.set(self, self.dat_arr177[1] as usize, 0);
      return Ok(self.dat_arr177[1] as i32);
    } else {
      let mut run_start226 = (var227 / 2) as i16;
      while run_start226 >= 1 {
        self.fn225(run_start226 as i32, dat_arr_cursor187, var227 as i16);
        run_start226 -= 1
      }
      let mut var289;
      loop {
        run_start226 = self.dat_arr177[1];
        if run_start226 < self.dat174 {
          dat_arr_cursor188.set(self, 0, run_start226 as u16);
          dat_arr_cursor188.shift(self, 1);
        }
        self.dat_arr177[1] = self.dat_arr177[var227];
        var227 -= 1;
        self.fn225(1, dat_arr_cursor187, var227 as i16);
        let run_length276 = self.dat_arr177[1];
        if run_length276 < self.dat174 {
          dat_arr_cursor188.set(self, 0, run_length276 as u16);
          dat_arr_cursor188.shift(self, 1);
        }
        var289 = var292;
        var292 = var292 + 1;
        dat_arr_cursor187.set(
          self,
          var289 as usize,
          dat_arr_cursor187.get(self, run_start226 as usize)
            + dat_arr_cursor187.get(self, run_length276 as usize),
        );
        self.dat_arr177[1] = var289 as i16;
        self.fn225(1, dat_arr_cursor187, var227 as i16);
        self.dat_arr189[var289 as usize] = run_start226 as u16;
        self.dat_arr190[var289 as usize] = run_length276 as u16;
        if !(var227 > 1) {
          break;
        }
      }
      dat_arr_cursor188.set_offset(self, dat_arr_cursor188_offset);
      self.fn228(var289, dat_arr_cursor178, dat_arr_cursor188);
      dat_arr_cursor188.set_offset(self, dat_arr_cursor188_offset);
      dat_arr_cursor178.set_offset(self, dat_arr_cursor178_offset);
      self.fn230(var212, dat_arr_cursor178, dat_arr_cursor188);
      return Ok(var289);
    };
  }
}
