use crate::compress::{RCompressData, Result};
use crate::consts::{CONST_N141_IS_511, CONST_N143_IS_9};
use std::io::Read;
use crate::support::BitwiseWrite;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn222(&mut self) -> Result<()> {
    let mut bits_to_load219 = CONST_N141_IS_511;
    while bits_to_load219 > 0 && self.dat_arr180[bits_to_load219 - 1] == 0 {
      bits_to_load219 -= 1
    }
    self.write_bits_to_buffer(CONST_N143_IS_9 as u16, bits_to_load219 as u16)?;
    let mut run_start226 = 0;
    while run_start226 < bits_to_load219 {
      let mut var289 = self.dat_arr180[run_start226] as i16;
      run_start226 = run_start226 + 1;
      if var289 == 0 {
        let mut var277 = 1;
        while (run_start226) < bits_to_load219 && self.dat_arr180[run_start226] == 0 {
          run_start226 += 1;
          var277 += 1
        }
        if var277 <= 2 {
          var289 = 0 as i16;
          while (var289) < var277 {
            let a1 = self.dat_arr181[0] as u16;
            let a2 = self.dat_arr194[0];
            self.write_bits_to_buffer(a1, a2)?;
            var289 += 1
          }
        } else if var277 <= 18 {
          let a1 = self.dat_arr181[1] as u16;
          let a2 = self.dat_arr194[1];
          self.write_bits_to_buffer(a1, a2)?;
          self.write_bits_to_buffer(4, (var277 - 3) as u16)?;
        } else if var277 == 19 {
          let a1 = self.dat_arr181[0] as u16;
          let a2 = self.dat_arr194[0];
          self.write_bits_to_buffer(a1, a2)?;
          let a1 = self.dat_arr181[1] as u16;
          let a2 = self.dat_arr194[1];
          self.write_bits_to_buffer(a1, a2)?;
          self.write_bits_to_buffer(4, 15)?;
        } else {
          let a1 = self.dat_arr181[2] as u16;
          let a2 = self.dat_arr194[2];
          self.write_bits_to_buffer(a1, a2)?;
          self.write_bits_to_buffer(CONST_N143_IS_9 as u16, (var277 - 20) as u16)?;
        }
      } else {
        let a1 = self.dat_arr181[var289 as usize + 2] as u16;
        let a2 = self.dat_arr194[var289 as usize + 2];
        self.write_bits_to_buffer(a1, a2)?;
      }
    }
    Ok(())
  }
}
