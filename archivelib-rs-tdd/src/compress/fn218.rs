use crate::compress::{RCompressData, Result};
use crate::support::BitwiseWrite;
use std::io::Read;

const USHRT_MAX: u16 = u16::max_value();

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn218(&mut self, mut bits_to_load219: i16, var220: i16, var221: i16) -> Result<()> {
    while bits_to_load219 > 0 && self.dat_arr181[bits_to_load219 as usize - 1] == 0 {
      bits_to_load219 -= 1
    }
    self
      .output_store
      .write_bits(bits_to_load219 as u32, var220 as usize)?;
    let mut run_start226: usize = 0;
    while run_start226 < bits_to_load219 as usize {
      let var289 = self.dat_arr181[run_start226] as u16;
      run_start226 = run_start226 + 1;
      if var289 <= 6 {
        self.output_store.write_bits(var289 as u32, 3 as usize)?;
      } else {
        self
          .output_store
          .write_bits((USHRT_MAX << 1) as u32, var289 as usize - 3 as usize)?;
      }
      if !(run_start226 == var221 as usize) {
        continue;
      }
      while (run_start226) < 6 && self.dat_arr181[run_start226] == 0 {
        run_start226 += 1
      }
      self
        .output_store
        .write_bits((run_start226 - 3) as u32, 2 as usize)?;
    }
    Ok(())
  }
}
