use std::convert::TryInto;
use std::io::Read;

use super::array_alias::ArrayAlias;
use crate::compress::{CompressU16ArrayAlias, CompressU8ArrayAlias, RCompressData, Result};
use crate::support::BitwiseWrite;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn211(
    &mut self,
    var212: i32,
    dat_arr_cursor187: &mut CompressU16ArrayAlias<'_>,
    dat_arr_cursor178: &mut CompressU8ArrayAlias<'_>,
    dat_arr_cursor188: &mut CompressU16ArrayAlias<'_>,
  ) -> Result<u32> {
    let dat_arr_cursor178_offset = dat_arr_cursor178.offset(self);
    let dat_arr_cursor188_offset = dat_arr_cursor188.offset(self);

    self.dat174 = cast!(var212 as i16);
    let mut var292 = cast!(var212 as u32);
    let mut var227 = 0;
    self.dat_arr177[1] = 0;
    for i in 0..cast!((self.dat174) as usize) {
      dat_arr_cursor178.set(self, i, 0);
      if 0 != dat_arr_cursor187.get(self, i) {
        var227 += 1;
        self.dat_arr177[var227] = i.try_into().unwrap();
      }
    }
    if var227 < 2 {
      dat_arr_cursor188.set(self, self.dat_arr177[1].try_into().unwrap(), 0);
      Ok(self.dat_arr177[1].try_into().unwrap())
    } else {
      let mut run_start226 = (var227 / 2).try_into().unwrap();
      while run_start226 >= 1 {
        self.fn225(
          i32::from(run_start226),
          dat_arr_cursor187,
          cast!(var227 as i16),
        );
        run_start226 -= 1
      }
      let mut var289;
      loop {
        run_start226 = self.dat_arr177[1];
        if run_start226 < self.dat174 {
          dat_arr_cursor188.set(self, 0, cast!(run_start226 as u16));
          dat_arr_cursor188.shift(self, 1);
        }
        self.dat_arr177[1] = self.dat_arr177[var227];
        var227 -= 1;
        self.fn225(1, dat_arr_cursor187, cast!(var227 as i16));
        let run_length276 = self.dat_arr177[1];
        if run_length276 < self.dat174 {
          dat_arr_cursor188.set(self, 0, cast!(run_length276 as u16));
          dat_arr_cursor188.shift(self, 1);
        }
        var289 = var292;
        var292 += 1;
        dat_arr_cursor187.set(
          self,
          cast!(var289 as usize),
          dat_arr_cursor187.get(self, cast!(run_start226 as usize))
            + dat_arr_cursor187.get(self, cast!(run_length276 as usize)),
        );
        self.dat_arr177[1] = cast!(var289 as i16);
        self.fn225(1, dat_arr_cursor187, cast!(var227 as i16));
        self.dat_arr189[cast!(var289 as usize)] = cast!(run_start226 as u16);
        self.dat_arr190[cast!(var289 as usize)] = cast!(run_length276 as u16);
        if var227 <= 1 {
          break;
        }
      }
      dat_arr_cursor188.set_offset(self, dat_arr_cursor188_offset);
      self.fn228(cast!(var289 as i32), dat_arr_cursor178, dat_arr_cursor188);
      dat_arr_cursor188.set_offset(self, dat_arr_cursor188_offset);
      dat_arr_cursor178.set_offset(self, dat_arr_cursor178_offset);
      self.fn230(var212, dat_arr_cursor178, dat_arr_cursor188);
      Ok(var289)
    }
  }
}
