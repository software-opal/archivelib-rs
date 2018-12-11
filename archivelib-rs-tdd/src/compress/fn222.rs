use crate::compress::{RCompressData, Result};
use crate::consts::{CONST_N141_IS_511, CONST_N143_IS_9};
use crate::support::BitwiseWrite;
use std::io::Read;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn222(&mut self) -> Result<()> {
    let mut bits_to_load219 = CONST_N141_IS_511;
    while bits_to_load219 > 0 && self.dat_arr180[bits_to_load219 - 1] == 0 {
      bits_to_load219 -= 1
    }
    self
      .output_store
      .write_bits(bits_to_load219 as u32, CONST_N143_IS_9 as usize)?;
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
            self.output_store.write_bits(a2 as u32, a1 as usize)?;
            var289 += 1
          }
        } else if var277 <= 18 {
          let a1 = self.dat_arr181[1] as u16;
          let a2 = self.dat_arr194[1];
          self.output_store.write_bits(a2 as u32, a1 as usize)?;
          self
            .output_store
            .write_bits((var277 - 3) as u32, 4 as usize)?;
        } else if var277 == 19 {
          let a1 = self.dat_arr181[0] as u16;
          let a2 = self.dat_arr194[0];
          self.output_store.write_bits(a2 as u32, a1 as usize)?;
          let a1 = self.dat_arr181[1] as u16;
          let a2 = self.dat_arr194[1];
          self.output_store.write_bits(a2 as u32, a1 as usize)?;
          self.output_store.write_bits(15 as u32, 4 as usize)?;
        } else {
          let a1 = self.dat_arr181[2] as u16;
          let a2 = self.dat_arr194[2];
          self.output_store.write_bits(a2 as u32, a1 as usize)?;
          self
            .output_store
            .write_bits((var277 - 20) as u32, CONST_N143_IS_9 as usize)?;
        }
      } else {
        let a1 = self.dat_arr181[var289 as usize + 2] as u16;
        let a2 = self.dat_arr194[var289 as usize + 2];
        self.output_store.write_bits(a2 as u32, a1 as usize)?;
      }
    }
    Ok(())
  }
}
