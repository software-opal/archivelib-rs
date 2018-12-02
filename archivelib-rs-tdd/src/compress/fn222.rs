use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn222(&mut self) {
    let mut run_start226: i16 = 0;
    let mut _289: i16 = 0;
    let mut bits_to_load219: i16 = 0;
    let mut _277: i16 = 0;
    bits_to_load219 = (127 * 2 + 1 + 1 + 256 - 3 + 1 + 1) as i16;
    while bits_to_load219 > 0 && *self.dat_arr180.offset((bits_to_load219 - 1) as isize) == 0 {
      bits_to_load219 -= 1
    }
    write_bits_to_buffer(data, 9, bits_to_load219 as u16);
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
          _289 = 0 as i16;
          while (_289) < _277 {
            write_bits_to_buffer(
              data,
              *self.dat_arr181.offset(0) as i32,
              *self.dat_arr194.offset(0),
            );
            _289 += 1
          }
        } else if _277 <= 18 {
          write_bits_to_buffer(
            data,
            *self.dat_arr181.offset(1) as i32,
            *self.dat_arr194.offset(1),
          );
          write_bits_to_buffer(data, 4, (_277 - 3) as u16);
        } else if _277 == 19 {
          write_bits_to_buffer(
            data,
            *self.dat_arr181.offset(0) as i32,
            *self.dat_arr194.offset(0),
          );
          write_bits_to_buffer(
            data,
            *self.dat_arr181.offset(1) as i32,
            *self.dat_arr194.offset(1),
          );
          write_bits_to_buffer(data, 4, 15 as u16);
        } else {
          write_bits_to_buffer(
            data,
            *self.dat_arr181.offset(2) as i32,
            *self.dat_arr194.offset(2),
          );
          write_bits_to_buffer(data, 9, (_277 - 20) as u16);
        }
      } else {
        write_bits_to_buffer(
          data,
          *self.dat_arr181.offset((_289 + 2) as isize) as i32,
          *self.dat_arr194.offset((_289 + 2) as isize),
        );
      }
    }
  }
}
