use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn218(&mut self, mut bits_to_load219: i16, mut _220: i16, mut _221: i16) {
    let mut run_start226: i16 = 0;
    let mut _289: i16 = 0;
    while bits_to_load219 > 0 && *self.dat_arr181.offset((bits_to_load219 - 1) as isize) == 0 {
      bits_to_load219 -= 1
    }
    write_bits_to_buffer(data, _220 as i32, bits_to_load219 as u16);
    run_start226 = 0 as i16;
    while (run_start226) < bits_to_load219 {
      let fresh0 = run_start226;
      run_start226 = run_start226 + 1;
      _289 = *self.dat_arr181.offset(fresh0 as isize) as i16;
      if _289 <= 6 {
        write_bits_to_buffer(data, 3, _289 as u16);
      } else {
        write_bits_to_buffer(data, _289 - 3, (32767 * 2 + 1 << 1) as u16);
      }
      if !(run_start226 == _221) {
        continue;
      }
      while (run_start226) < 6 && *self.dat_arr181.offset(run_start226 as isize) == 0 {
        run_start226 += 1
      }
      write_bits_to_buffer(data, 2, (run_start226 - 3) as u16);
    }
  }
}
