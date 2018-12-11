use crate::compress::{RCompressData, Result};
use std::io::Read;
use crate::support::BitwiseWrite;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn224(&mut self, var204: u16) -> Result<()> {
    let mut byte_or_run_length203 = 0;
    let mut var457 = var204;
    while 0 != var457 {
      byte_or_run_length203 += 1;
      var457 = (var457 >> 1) as u16
    }
    let a1 = self.dat_arr181[byte_or_run_length203];
    let a2 = self.dat_arr194[byte_or_run_length203];
    self.output_store.write_bits(a2 as u32, a1 as usize)?;
    if byte_or_run_length203 > 1 {
      self.output_store.write_bits(var204 as u32, byte_or_run_length203 - 1 as usize)?;
    }
    Ok(())
  }
}
