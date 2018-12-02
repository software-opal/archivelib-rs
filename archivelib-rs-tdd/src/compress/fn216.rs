use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn216(&mut self, mut _217: *mut u16) {
    let mut run_start226: i16 = 0;
    let mut _289: i16 = 0;
    let mut bits_to_load219: i16 = 0;
    let mut _277: i16 = 0;
    run_start226 = 0 as i16;
    while (run_start226) < 16 + 3 {
      *_217.offset(run_start226 as isize) = 0 as u16;
      run_start226 += 1
    }
    bits_to_load219 = (127 * 2 + 1 + 1 + 256 - 3 + 1 + 1) as i16;
    while bits_to_load219 > 0 && *self.dat_arr180.offset((bits_to_load219 - 1) as isize) == 0 {
      bits_to_load219 -= 1
    }
    run_start226 = 0 as i16;
    while (run_start226) < bits_to_load219 {
      let fresh0 = run_start226;
      run_start226 = run_start226 + 1;
      _289 = *self.dat_arr180.offset(fresh0 as isize) as i16;
      if _289 == 0 {
        _277 = 1 as i16;
        while (run_start226) < bits_to_load219
          && *self.dat_arr180.offset(run_start226 as isize) == 0
        {
          run_start226 += 1;
          _277 += 1
        }
        if _277 <= 2 {
          let ref mut fresh1 = *_217.offset(0);
          *fresh1 = (*fresh1 + _277) as u16
        } else if _277 <= 18 {
          let ref mut fresh2 = *_217.offset(1);
          *fresh2 = (*fresh2).wrapping_add(1)
        } else if _277 == 19 {
          let ref mut fresh3 = *_217.offset(0);
          *fresh3 = (*fresh3).wrapping_add(1);
          let ref mut fresh4 = *_217.offset(1);
          *fresh4 = (*fresh4).wrapping_add(1)
        } else {
          let ref mut fresh5 = *_217.offset(2);
          *fresh5 = (*fresh5).wrapping_add(1)
        }
      } else {
        let ref mut fresh6 = *_217.offset((_289 + 2) as isize);
        *fresh6 = (*fresh6).wrapping_add(1)
      }
    }
  }
}
