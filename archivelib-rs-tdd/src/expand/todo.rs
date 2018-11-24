use crate::expand::reader::BitwiseReadAheadRead;
use crate::expand::{RExpandData, Result};
use crate::support::{BitwiseRead, BitwiseWrite};

impl<R: BitwiseReadAheadRead, W: BitwiseWrite> RExpandData<R, W> {
  pub fn calculate_run_offset(&mut self) -> Result<u16> {
    unimplemented!();
  }
  pub fn fn253(&mut self, _254: i16, _220: i16, _221: i16) -> Result<()> {
    unimplemented!();
  }
  pub fn fn255(&mut self) -> Result<()> {
    unimplemented!();
  }
  pub fn get_bits(&mut self, _bits_to_load219: u8) -> Result<u16> {
    unimplemented!();
  }
}
