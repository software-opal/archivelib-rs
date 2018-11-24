use super::fn258::Fn258Mode;
use crate::consts::{CONST_N141_IS_511, CONST_N143_IS_9, CONST_N145_IS_19, CONST_N148_IS_4096};
use crate::expand::{RExpandData, Result};
use crate::support::{BitwiseReadAheadRead, BitwiseWrite};

impl<R: BitwiseReadAheadRead, W: BitwiseWrite> RExpandData<R, W> {
  pub fn fn255(&mut self) -> Result<()> {
    let mut run_start226: i16 = 0;
    let mut byte_or_run_length203: i16 = 0;
    let mut bits_to_load219: i16 = 0;
    let mut _283: u16 = 0;
    bits_to_load219 = self.get_bits(CONST_N143_IS_9 as i16)? as i16;
    if bits_to_load219 == 0 {
      byte_or_run_length203 = self.get_bits(CONST_N143_IS_9 as i16)? as i16;
      run_start226 = 0 as i16;
      while (run_start226) < CONST_N141_IS_511 as i16 {
        (self).dat_arr180[run_start226 as usize] = 0 as u8;
        run_start226 += 1
      }
      run_start226 = 0 as i16;
      while (run_start226) < CONST_N148_IS_4096 as i16 {
        (self).dat_arr240[run_start226 as usize] = byte_or_run_length203 as u16;
        run_start226 += 1
      }
    } else {
      run_start226 = 0 as i16;
      while (run_start226) < bits_to_load219 {
        byte_or_run_length203 = (self).dat_arr241[((self).bits182 >> 8) as usize] as i16;
        if byte_or_run_length203 >= CONST_N145_IS_19 as i16 {
          _283 = (1 << 7) as u16;
          loop {
            if 0 != (self).bits182 & _283 {
              byte_or_run_length203 = (self).dat_arr190[byte_or_run_length203 as usize] as i16
            } else {
              byte_or_run_length203 = (self).dat_arr189[byte_or_run_length203 as usize] as i16
            }
            _283 = (_283 >> 1) as u16;
            if !(byte_or_run_length203 >= CONST_N145_IS_19 as i16) {
              break;
            }
          }
        }
        let bits = (self).dat_arr181[byte_or_run_length203 as usize] as i16;
        self.read_bits(bits)?;
        if byte_or_run_length203 <= 2 {
          if byte_or_run_length203 == 0 {
            byte_or_run_length203 = 1 as i16
          } else if byte_or_run_length203 == 1 {
            byte_or_run_length203 = (self.get_bits(4)? + 3) as i16
          } else {
            byte_or_run_length203 = (self.get_bits(CONST_N143_IS_9 as i16)? + 20) as i16
          }
          loop {
            byte_or_run_length203 -= 1;
            if !(byte_or_run_length203 >= 0) {
              break;
            }
            let fresh0 = run_start226;
            run_start226 = run_start226 + 1;
            (self).dat_arr180[fresh0 as usize] = 0 as u8
          }
        } else {
          let fresh1 = run_start226;
          run_start226 = run_start226 + 1;
          (self).dat_arr180[fresh1 as usize] = (byte_or_run_length203 - 2) as u8
        }
      }
      while (run_start226) < 127 * 2 + 1 + 1 + 256 - 3 + 1 + 1 {
        let fresh2 = run_start226;
        run_start226 = run_start226 + 1;
        (self).dat_arr180[fresh2 as usize] = 0 as u8
      }
      self.fn258(
        Fn258Mode::Fn255,
        CONST_N141_IS_511 as u32,
        12,
        CONST_N148_IS_4096 as u16,
      )?;
    };
    Ok(())
  }
}
