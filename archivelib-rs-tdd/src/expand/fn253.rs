use super::fn258::Fn258Mode;
use crate::consts::CONST_N149_IS_256;
use crate::expand::{RExpandData, Result};
use crate::support::{BitwiseReadAheadRead, BitwiseWrite};

impl<R: BitwiseReadAheadRead, W: BitwiseWrite> RExpandData<R, W> {
  pub fn fn253(&mut self, mut _254: i16, mut _220: i16, mut _221: i16) -> Result<()> {
    let mut run_start226: i16 = 0;
    let mut byte_or_run_length203: i16 = 0;
    let mut bits_to_load219: i16 = 0;
    let mut _283: u16 = 0;
    bits_to_load219 = self.get_bits(_220 as i16)? as i16;
    if bits_to_load219 == 0 {
      byte_or_run_length203 = self.get_bits(_220 as i16)? as i16;
      run_start226 = 0 as i16;
      while (run_start226) < _254 {
        self.dat_arr181[run_start226 as usize] = 0 as u8;
        run_start226 += 1
      }
      run_start226 = 0 as i16;
      while (run_start226) < 256 {
        self.dat_arr241[run_start226 as usize] = byte_or_run_length203 as u16;
        run_start226 += 1
      }
    } else {
      run_start226 = 0 as i16;
      while (run_start226) < bits_to_load219 {
        byte_or_run_length203 = ((self).bits182 >> 13) as i16;
        if byte_or_run_length203 == 7 {
          let mut bytes_read: usize = 3;
          _283 = (1 << 12) as u16;
          while 0 != _283 & (self).bits182 {
            _283 = (_283 >> 1) as u16;
            byte_or_run_length203 += 1;
            bytes_read = bytes_read.wrapping_add(1)
          }
          // +1 for the final bit that was zero
          self.read_bits(bytes_read.wrapping_add(1) as i16)?;
        } else {
          self.read_bits(3)?;
        }
        let fresh0 = run_start226;
        run_start226 = run_start226 + 1;
        self.dat_arr181[fresh0 as usize] = byte_or_run_length203 as u8;
        if !(run_start226 == _221) {
          continue;
        }
        byte_or_run_length203 = self.get_bits(2)? as i16;
        while byte_or_run_length203 > 0 {
          let fresh1 = run_start226;
          run_start226 = run_start226 + 1;
          (self).dat_arr181[fresh1 as usize] = 0 as u8;
          byte_or_run_length203 -= 1
        }
      }
      while (run_start226) < _254 {
        (self).dat_arr181[run_start226 as usize] = 0 as u8;
        run_start226 += 1
      }
      self.fn258(Fn258Mode::Fn255, _254 as u32, 8, CONST_N149_IS_256 as u16)?;
    };
    Ok(())
  }
}
