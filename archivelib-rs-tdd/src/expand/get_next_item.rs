use crate::consts::{
  CONST_N141_IS_511, CONST_N142_IS_15, CONST_N145_IS_19, CONST_N147_IS_5, CONST_N540_IS_5,
};
use crate::expand::{ExpandError, RExpandData, Result};
use crate::support::{BitRead, BitwiseWrite};

impl<R: BitRead, W: BitwiseWrite> RExpandData<R, W> {
  pub fn get_next_item(&mut self) -> Result<u16> {
    let mut run_length276: u16 = 0;
    let mut _283: u16 = 0;
    if self.items_until_next_header == 0 {
      // This is the first 2 bytes in the file, and it represents the number of
      // calls that this header can handle. It's not exactly the number of bytes
      // because we read a variable number of bits per call.
      self.items_until_next_header = self.get_bits(16)? as usize;
      self.fn253(CONST_N145_IS_19 as i16, CONST_N147_IS_5 as i16, 3)?;
      self.fn255()?;
      self.fn253(CONST_N142_IS_15 as i16, CONST_N540_IS_5 as i16, -1)?;
    }
    self.items_until_next_header = self.items_until_next_header - 1;
    run_length276 = self.dat_arr240[(self.bits182 >> 4) as usize];
    // run_length276 <= 0xFF are the uncompressed bits.
    // 0x100 <= run_length276 <= 0x1FE are runs (run_length276 - 0x100 + 3) bits
    // long
    if run_length276 as usize >= CONST_N141_IS_511 {
      // No test cases exercise this condition.
      _283 = (1 << 3) as u16;
      loop {
        if 0 != self.bits182 & _283 {
          run_length276 = self.dat_arr190[run_length276 as usize]
        } else {
          run_length276 = self.dat_arr189[run_length276 as usize]
        }
        _283 = (_283 >> 1) as u16;
        if !(run_length276 as usize >= CONST_N141_IS_511) {
          break;
        }
      }
    }
    let bits = self.dat_arr180[run_length276 as usize] as i16;
    self.read_bits(bits)?;
    return Ok(run_length276);
  }
}
