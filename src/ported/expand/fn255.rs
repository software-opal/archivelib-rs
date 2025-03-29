use super::fn258::Fn258Mode;
use super::{RExpandData, Result};
use crate::ported::consts::{
  CONST_N141_IS_511, CONST_N143_IS_9, CONST_N145_IS_19, CONST_N148_IS_4096,
};
use crate::ported::support::BitRead;
use std::io::Write;

impl<R: BitRead, W: Write> RExpandData<R, W> {
  pub fn fn255(&mut self) -> Result<()> {
    let mut var283: u16;
    let bits_to_load219: i16 = self.get_bits(cast!(CONST_N143_IS_9 as i16))? as i16;
    if bits_to_load219 == 0 {
      let byte_or_run_length203: i16 = self.get_bits(cast!(CONST_N143_IS_9 as i16))? as i16;
      let mut run_start226: i16 = 0 as i16;
      while (run_start226) < cast!(CONST_N141_IS_511 as i16) {
        self.dat_arr180[cast!(run_start226 as usize)] = 0_u8;
        run_start226 += 1
      }
      run_start226 = 0 as i16;
      while (run_start226) < cast!(CONST_N148_IS_4096 as i16) {
        self.dat_arr240[cast!(run_start226 as usize)] = cast!(byte_or_run_length203 as u16);
        run_start226 += 1
      }
    } else {
      let mut run_start226: i16 = 0 as i16;
      while (run_start226) < bits_to_load219 {
        let mut byte_or_run_length203: i16 = self.dat_arr241[(self.bits182 >> 8) as usize] as i16;
        if byte_or_run_length203 >= cast!(CONST_N145_IS_19 as i16) {
          var283 = (1 << 7) as u16;
          loop {
            if 0 != self.bits182 & var283 {
              byte_or_run_length203 = self.dat_arr190[cast!(byte_or_run_length203 as usize)] as i16
            } else {
              byte_or_run_length203 = self.dat_arr189[cast!(byte_or_run_length203 as usize)] as i16
            }
            var283 = (var283 >> 1) as u16;
            if byte_or_run_length203 < cast!(CONST_N145_IS_19 as i16) {
              break;
            }
          }
        }
        let bits = i16::from(self.dat_arr181[cast!(byte_or_run_length203 as usize)]);
        self.read_bits(bits)?;
        if byte_or_run_length203 <= 2 {
          if byte_or_run_length203 == 0 {
            byte_or_run_length203 = 1 as i16
          } else if byte_or_run_length203 == 1 {
            byte_or_run_length203 = (self.get_bits(4)? + 3) as i16
          } else {
            byte_or_run_length203 = (self.get_bits(cast!(CONST_N143_IS_9 as i16))? + 20) as i16
          }
          let mut count = 0;
          loop {
            byte_or_run_length203 -= 1;
            if byte_or_run_length203 < 0 {
              break;
            }
            let fresh0 = run_start226;
            run_start226 += 1;
            self.dat_arr180[cast!(fresh0 as usize)] = 0_u8;
            count += 1;
          }
        } else {
          let fresh1 = run_start226;
          run_start226 += 1;
          self.dat_arr180[cast!(fresh1 as usize)] = (byte_or_run_length203 - 2) as u8
        }
      }
      while (run_start226) < (cast!(CONST_N141_IS_511 as i16)) {
        let fresh2 = run_start226;
        run_start226 += 1;
        self.dat_arr180[cast!(fresh2 as usize)] = 0_u8
      }
      self.fn258(
        Fn258Mode::Fn255,
        CONST_N141_IS_511,
        12,
        cast!(CONST_N148_IS_4096 as u16),
      )?;
    };
    Ok(())
  }
}
