use crate::compress::RCompressData;
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn225(
    &mut self,
    run_start226: i32,
    var187: &mut [u16], // Can be removed?
    var177: &mut [i16], // Can be removed?
    var227: i16,
  ) {
    let mut run_start: usize = run_start226 as usize;
    let var289 = var177[run_start];
    loop {
      let mut run_length276 = 2 * run_start;
      if !(run_length276 <= var227 as usize) {
        break;
      }
      if run_length276 < var227 as usize
        && var187[var177[run_length276] as usize] > var187[var177[run_length276 + 1] as usize]
      {
        run_length276 += 1
      }
      if var187[var289 as usize] <= var187[var177[run_length276] as usize] {
        break;
      }
      var177[run_start] = var177[run_length276];
      run_start = run_length276
    }
    var177[run_start] = var289 as i16;
  }
}
