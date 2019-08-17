use std::convert::{TryFrom, TryInto};
use std::io::Read;

use crate::compress::{RCompressData, Result};
use crate::support::BitwiseWrite;

const CHAR_BIT: usize = 8;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn202(&mut self, byte_or_run_length203: u16, arg204: u16) -> Result<()> {
    self.bitwise_counter185 = (self.bitwise_counter185 >> 1).try_into().unwrap();
    if self.bitwise_counter185 == 0 {
      self.bitwise_counter185 = cast!((1 << (CHAR_BIT - 1)) as u16);
      if self.array165_counter >= 8162 {
        self.fn207()?;
        self.array165_counter = 0;
      }
      self.array165_tmp_counter186 = self.array165_counter;
      self.array165_counter += 1;
      self.dat_arr165[self.array165_tmp_counter186] = 0;
    }
    self.array165_counter = pure_fn202_part_2(
      &mut self.dat_arr165,
      &mut self.dat_arr191,
      &mut self.dat_arr193,
      self.array165_counter,
      self.array165_tmp_counter186,
      self.bitwise_counter185,
      byte_or_run_length203,
      arg204,
    );
    Ok(())
  }
}

pub fn pure_fn202_part_2(
  dat_arr165: &mut [u8],
  dat_arr191: &mut [u16],
  dat_arr193: &mut [u16],
  mut array165_counter: usize,
  array165_tmp_counter186: usize,
  bitwise_counter185: u16,
  byte_or_run_length203: u16,
  arg204: u16,
) -> usize {
  dat_arr165[array165_counter] = cast_trunc!(byte_or_run_length203 as u8);
  array165_counter += 1;
  dat_arr191[cast!(byte_or_run_length203 as usize)] += 1;
  if byte_or_run_length203 >= (1 << CHAR_BIT) {
    dat_arr165[array165_tmp_counter186] |= cast!(bitwise_counter185 as u8);
    dat_arr165[array165_counter] = cast_trunc!(arg204 as u8);
    array165_counter += 1;
    dat_arr165[array165_counter] = cast!((arg204 >> 8) as u8);
    array165_counter += 1;
    let counter = 16 - arg204.leading_zeros();
    dat_arr193[cast!(counter as usize)] += 1;
  }
  array165_counter
}
