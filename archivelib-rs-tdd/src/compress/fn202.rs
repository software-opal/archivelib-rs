use crate::compress::{CompressError, RCompressData, Result};
use crate::support::BitwiseWrite;
use std::io::Read;

const CHAR_BIT: usize = 8;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn202(&mut self, byte_or_run_length203: u16, arg204: u16) -> Result<()> {
    self.bitwise_counter185 = (self.bitwise_counter185 >> 1) as u16;
    if self.bitwise_counter185 == 0 {
      self.bitwise_counter185 = (1 << CHAR_BIT - 1) as u16;
      if self.array165_counter >= 8162 {
        self.fn207()?;
        self.array165_counter = 0;
      }
      self.array165_tmp_counter186 = self.array165_counter;
      self.array165_counter += 1;
      self.dat_arr165[self.array165_tmp_counter186] = 0 as u8;
    }
    self.dat_arr165[self.array165_counter] = byte_or_run_length203 as u8;
    self.array165_counter += 1;
    self.dat_arr191[byte_or_run_length203 as usize] += 1;
    if byte_or_run_length203 >= (1 << CHAR_BIT) {
      self.dat_arr165[self.array165_tmp_counter186] |= self.bitwise_counter185 as u8;
      self.dat_arr165[self.array165_counter] = arg204 as u8;
      self.array165_counter += 1;
      self.dat_arr165[self.array165_counter] = (arg204 >> 8) as u8;
      self.array165_counter += 1;
      let counter = 16 - arg204.leading_zeros();
      self.dat_arr193[counter as usize] += 1;
    }
    Ok(())
  }
}

pub fn pure_fn202_part_2(dat_arr165, dat_arr191, dat_arr193,
array165_counter,array165_tmp_counter186, bitwise_counter185, byte_or_run_length203,arg204
) {

}
