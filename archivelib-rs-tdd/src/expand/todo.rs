use crate::expand::{RExpandData, Result};
use crate::support::{BitwiseRead, BitwiseWrite};

impl<R: BitwiseRead, W: BitwiseWrite> RExpandData<R, W> {
  fn get_next_item(self: &mut Self) -> u16 {
    unimplemented!();
  }
  fn calculate_run_offset(self: &mut Self) -> u16 {
    unimplemented!();
  }
  fn read_bits(self: &mut Self, bits_to_load219: i32) {
    unimplemented!();
  }
}
