use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn225(
    &mut self,
    mut run_start226: i32,
    mut _187: *mut u16,
    mut _177: *mut i16,
    mut _227: i16,
  ) {
    let mut run_length276: i32 = 0;
    let mut _289: i32 = 0;
    _289 = *_177.offset(run_start226 as isize) as i32;
    loop {
      run_length276 = 2 * run_start226;
      if !(run_length276 <= _227) {
        break;
      }
      if run_length276 < _227
        && *_187.offset(*_177.offset(run_length276 as isize) as isize)
          > *_187.offset(*_177.offset((run_length276 + 1) as isize) as isize)
      {
        run_length276 += 1
      }
      if *_187.offset(_289 as isize) <= *_187.offset(*_177.offset(run_length276 as isize) as isize)
      {
        break;
      }
      *_177.offset(run_start226 as isize) = *_177.offset(run_length276 as isize);
      run_start226 = run_length276
    }
    *_177.offset(run_start226 as isize) = _289 as u16 as i16;
  }
}
