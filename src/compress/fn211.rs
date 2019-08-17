use std::convert::{TryFrom, TryInto};
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

    self.dat174 = i16::try_from(var212).unwrap();
    let mut var292 = u32::try_from(var212).unwrap();
    let mut var227 = 0;
    self.dat_arr177[1] = 0;
    for i in 0..(usize::try_from(self.dat174).unwrap()) {
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
          i16::try_from(var227).unwrap(),
        );
        run_start226 -= 1
      }
      let mut var289;
      loop {
        run_start226 = self.dat_arr177[1];
        if run_start226 < self.dat174 {
          dat_arr_cursor188.set(self, 0, u16::try_from(run_start226).unwrap());
          dat_arr_cursor188.shift(self, 1);
        }
        self.dat_arr177[1] = self.dat_arr177[var227];
        var227 -= 1;
        self.fn225(1, dat_arr_cursor187, i16::try_from(var227).unwrap());
        let run_length276 = self.dat_arr177[1];
        if run_length276 < self.dat174 {
          dat_arr_cursor188.set(self, 0, u16::try_from(run_length276).unwrap());
          dat_arr_cursor188.shift(self, 1);
        }
        var289 = var292;
        var292 += 1;
        dat_arr_cursor187.set(
          self,
          usize::try_from(var289).unwrap(),
          dat_arr_cursor187.get(self, usize::try_from(run_start226).unwrap())
            + dat_arr_cursor187.get(self, usize::try_from(run_length276).unwrap()),
        );
        self.dat_arr177[1] = i16::try_from(var289).unwrap();
        self.fn225(1, dat_arr_cursor187, i16::try_from(var227).unwrap());
        self.dat_arr189[usize::try_from(var289).unwrap()] = u16::try_from(run_start226).unwrap();
        self.dat_arr190[usize::try_from(var289).unwrap()] = u16::try_from(run_length276).unwrap();
        if var227 <= 1 {
          break;
        }
      }
      dat_arr_cursor188.set_offset(self, dat_arr_cursor188_offset);
      self.fn228(
        i32::try_from(var289).unwrap(),
        dat_arr_cursor178,
        dat_arr_cursor188,
      );
      dat_arr_cursor188.set_offset(self, dat_arr_cursor188_offset);
      dat_arr_cursor178.set_offset(self, dat_arr_cursor178_offset);
      self.fn230(var212, dat_arr_cursor178, dat_arr_cursor188);
      Ok(var289)
    }
  }
}
