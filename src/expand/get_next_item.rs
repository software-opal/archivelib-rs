use crate::consts::{
  CONST_N141_IS_511, CONST_N142_IS_15, CONST_N145_IS_19, CONST_N147_IS_5, CONST_N540_IS_5,
};
use crate::expand::{RExpandData, Result};
use crate::support::BitRead;
use std::io::Write;

impl<R: BitRead, W: Write> RExpandData<R, W> {
  pub fn get_next_item(&mut self) -> Result<u16> {
    if self.items_until_next_header == 0 {
      // This is the first 2 bytes in the file, and it represents the number of
      // calls that this header can handle. It's not exactly the number of bytes
      // because we read a variable number of bits per call.
      self.items_until_next_header = self.get_bits(16)? as usize;
      self.fn253(CONST_N145_IS_19 as i16, CONST_N147_IS_5 as i16, 3)?;

      println!("dat_arr180: {:X?}", self.dat_arr180);
      println!("dat_arr181: {:X?}", self.dat_arr181);
      println!("dat_arr189: {:X?}", self.dat_arr189);
      println!("dat_arr190: {:X?}", self.dat_arr190);
      println!("dat_arr240: {:X?}", self.dat_arr240);
      println!("dat_arr241: {:X?}", self.dat_arr241);
      self.fn255()?;
      self.fn253(CONST_N142_IS_15 as i16, CONST_N540_IS_5 as i16, -1)?;
    }
    self.items_until_next_header -= 1;
    let mut run_length276 = self.dat_arr240[(self.bits182 >> 4) as usize];
    // run_length276 <= 0xFF are the uncompressed bits.
    // 0x100 <= run_length276 <= 0x1FE are runs (run_length276 - 0x100 + 3) bits
    // long
    if run_length276 as usize >= CONST_N141_IS_511 {
      assert!(false);
      // No test cases exercise this condition.
      let mut var283 = 1u16 << 3;
      loop {
        if 0 != self.bits182 & var283 {
          run_length276 = self.dat_arr190[run_length276 as usize]
        } else {
          run_length276 = self.dat_arr189[run_length276 as usize]
        }
        var283 >>= 1;
        if (run_length276 as usize) < CONST_N141_IS_511 {
          break;
        }
      }
    }
    let bits = i16::from(self.dat_arr180[run_length276 as usize]);
    self.read_bits(bits)?;
    Ok(run_length276)
  }
}
