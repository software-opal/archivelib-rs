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
    self.write_bits_to_buffer(a1 as u16, a2)?;
    if byte_or_run_length203 > 1 {
      self.write_bits_to_buffer(byte_or_run_length203 as u16 - 1, var204)?;
    }
    Ok(())
  }
}
