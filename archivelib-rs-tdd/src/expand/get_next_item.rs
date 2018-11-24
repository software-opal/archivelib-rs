use crate::consts::{CONST_N141_IS_511, CONST_N142_IS_15, CONST_N145_IS_19};
use crate::expand::{RExpandData, Result};
use crate::support::{BitwiseRead, BitwiseWrite};

impl<R: BitwiseRead, W: BitwiseWrite> RExpandData<R, W> {
  pub fn get_next_item(&mut self) -> Result<u16> {
    let mut run_length276: u16 = 0;
    if self.items_until_next_header == 0 {
      // This is the first 2 bytes in the file, and it represents the number of
      // calls that this header can handle. It's not exactly the number of bytes
      // because we read a variable number of bits per call.
      self.items_until_next_header = self.input_store.read_bits(16)? as usize;
      self.fn253(CONST_N145_IS_19 as i16, 5, 3)?;
      self.fn255()?;
      self.fn253(CONST_N142_IS_15 as i16, 5, -1)?;
    }
    self.items_until_next_header = self.items_until_next_header - 1;
    run_length276 = self.dat_arr240[self.input_store.read_bits(12)? as usize];
    // run_length276 <= 0xFF are the uncompressed bits.
    // 0x100 <= run_length276 <= 0x1FE are runs (run_length276 - 0x100 + 3) bits
    // long
    if run_length276 >= CONST_N141_IS_511 as u16 {
      // No test cases exercise this condition.
      loop {
        if self.input_store.read_bit()? {
          run_length276 = self.dat_arr190[run_length276 as usize];
        } else {
          run_length276 = self.dat_arr189[run_length276 as usize];
        }
        if !(run_length276 >= CONST_N141_IS_511 as u16) {
          break;
        }
      }
    }
    return Ok(run_length276);
  }
}
