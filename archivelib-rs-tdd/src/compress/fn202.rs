use crate::compress::{CompressError, RCompressData, Result};
use std::io::{Read, Write};

const CHAR_BIT: usize = 8;

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn202(&mut self, byte_or_run_length203: u16, mut arg204: u16) -> Result<()> {
    self.bitwise_counter185 = (self.bitwise_counter185 >> 1) as u16;
    if self.bitwise_counter185 == 0 {
      self.bitwise_counter185 = (1 << CHAR_BIT - 1) as u16;
      if self.array165_counter >= self.dat183_IS_CONST_8162 {
        self.fn207()?;
        if !self.uncompressible {
          return Err(CompressError::InputUncompressable);
        } else {
          self.array165_counter = 0 as u16
        }
      }
      self.array165_tmp_counter186 = self.array165_counter;
      self.array165_counter += 1;
      self.dat_arr165[self.array165_tmp_counter186 as usize] = 0 as u8;
    }
    self.dat_arr165[self.array165_counter as usize] = byte_or_run_length203 as u8;
    self.array165_counter += 1;
    self.dat_arr191[byte_or_run_length203 as usize] += 1;
    if byte_or_run_length203 >= (1 << CHAR_BIT) {
      self.dat_arr165[self.array165_tmp_counter186 as usize] |= self.bitwise_counter185 as u8;
      self.dat_arr165[self.array165_counter as usize] = arg204 as u8;
      self.array165_counter += 1;
      self.dat_arr165[self.array165_counter as usize] = (arg204 >> 8) as u8;
      self.array165_counter += 1;
      // TODO: Simplify?
      let mut counter = 0;
      let expected_counter = 16 - arg204.leading_zeros();
      while 0 != arg204 {
        counter += 1;
        arg204 = (arg204 >> 1) as u16
      }
      assert_eq!(counter, expected_counter);
      self.dat_arr193[counter as usize] += 1;
    }
    Ok(())
  }
}
