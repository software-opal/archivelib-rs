use crate::compress::{CompressU16ArrayAlias, RCompressData};
use crate::support::ArrayAlias;
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn225(
    &mut self,
    run_start226: i32,
    var187: &CompressU16ArrayAlias, // Can be removed?
    // self.dat_arr177: &mut [i16], // Can be removed?  &mut self.dat_arr177,
    var227: i16,
  ) {
    let mut run_start: usize = run_start226 as usize;
    let var289 = self.dat_arr177[run_start];
    loop {
      let mut run_length276 = 2 * run_start;
      if !(run_length276 <= var227 as usize) {
        break;
      }
      if run_length276 < var227 as usize
        && var187.get(self, self.dat_arr177[run_length276] as usize)
          > var187.get(self, self.dat_arr177[run_length276 + 1] as usize)
      {
        run_length276 += 1
      }
      if var187.get(self, var289 as usize)
        <= var187.get(self, self.dat_arr177[run_length276] as usize)
      {
        break;
      }
      self.dat_arr177[run_start] = self.dat_arr177[run_length276];
      run_start = run_length276
    }
    self.dat_arr177[run_start] = var289 as i16;
  }
}
