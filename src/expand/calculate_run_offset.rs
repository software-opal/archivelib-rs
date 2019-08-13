use crate::consts::CONST_N142_IS_15;
use crate::expand::{RExpandData, Result};
use crate::support::BitRead;
use std::io::Write;

impl<R: BitRead, W: Write> RExpandData<R, W> {
  #![allow(unused_assignments)]
  pub fn calculate_run_offset(&mut self) -> Result<u16> {
    let mut run_length276 = self.dat_arr241[(self.bits182 >> 8) as usize];
    let mut var283 = (1 << 7) as u16;
    while run_length276 >= CONST_N142_IS_15 as u16 {
      if 0 != self.bits182 & var283 {
        run_length276 = self.dat_arr190[run_length276 as usize];
      } else {
        run_length276 = self.dat_arr189[run_length276 as usize];
      }
      var283 >>= 1;
      panic!();
    }
    let bits = i16::from(self.dat_arr181[run_length276 as usize]);
    self.read_bits(bits)?;
    if run_length276 != 0 {
      run_length276 -= 1;
      run_length276 = (1 << run_length276) + self.get_bits(run_length276 as i16)?;
    }
    Ok(run_length276)
  }
}
