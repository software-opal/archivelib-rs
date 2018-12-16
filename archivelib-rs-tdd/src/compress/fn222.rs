use crate::compress::{RCompressData, Result};
use crate::consts::{CONST_N141_IS_511, CONST_N143_IS_9};
use crate::support::BitwiseWrite;
use std::io::Read;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn222(&mut self) -> Result<()> {
    pure_fn222(
      &mut self.output_store,
      &self.dat_arr180,
      &self.dat_arr181,
      &self.dat_arr194,
    )
  }
}

pub fn pure_fn222<W>(out: &mut W, arr180: &[u8], arr181: &[u8], arr194: &[u16]) -> Result<()>
where
  W: BitwiseWrite + Sized,
{
  let mut bits_to_load219 = CONST_N141_IS_511;
  while bits_to_load219 > 0 && arr180[bits_to_load219 - 1] == 0 {
    bits_to_load219 -= 1
  }
  out.write_bits(bits_to_load219, CONST_N143_IS_9)?;
  let mut run_start226 = 0;
  while run_start226 < bits_to_load219 {
    let mut var289 = arr180[run_start226] as usize;
    run_start226 = run_start226 + 1;
    if var289 == 0 {
      let mut var277 = 1;
      while (run_start226) < bits_to_load219 && arr180[run_start226] == 0 {
        run_start226 += 1;
        var277 += 1
      }
      if var277 <= 2 {
        for _ in 0..var277 {
          out.write_bits(arr194[0], arr181[0])?;
        }
      } else if var277 <= 18 {
        out.write_bits(arr194[1], arr181[1])?;
        out.write_bits((var277 - 3), 4)?;
      } else if var277 == 19 {
        out.write_bits(arr194[0], arr181[1])?;
        out.write_bits(arr194[1], arr181[1])?;
        out.write_bits(15, 4)?;
      } else {
        out.write_bits(arr194[2], arr181[2])?;
        out.write_bits((var277 - 20), CONST_N143_IS_9)?;
      }
    } else {
      out.write_bits(arr194[var289 + 2], arr181[var289 + 2])?;
    }
  }
  Ok(())
}
