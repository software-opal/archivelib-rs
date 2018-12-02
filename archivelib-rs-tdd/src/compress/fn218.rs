use crate::compress::{RCompressData, Result};
use std::io::{Read, Write};

const USHRT_MAX: u16 = u16::max_value();

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn218(&mut self, mut bits_to_load219: i16, var220: i16, var221: i16) -> Result<()> {
    while bits_to_load219 > 0 && self.dat_arr181[bits_to_load219 as usize - 1] == 0 {
      bits_to_load219 -= 1
    }
    self.write_bits_to_buffer(var220 as u16, bits_to_load219 as u16)?;
    let mut run_start226 = 0;
    while (run_start226) < bits_to_load219 {
      let var289 = self.dat_arr181[run_start226 as usize] as u16;
      run_start226 = run_start226 + 1;
      if var289 <= 6 {
        self.write_bits_to_buffer(3, var289)?;
      } else {
        self.write_bits_to_buffer(var289 - 3, (USHRT_MAX << 1) as u16)?;
      }
      if !(run_start226 == var221) {
        continue;
      }
      while (run_start226) < 6 && self.dat_arr181[run_start226 as usize] == 0 {
        run_start226 += 1
      }
      self.write_bits_to_buffer(2, (run_start226 - 3) as u16)?;
    }
    Ok(())
  }
}
