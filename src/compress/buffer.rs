use std::convert::{TryInto};
use std::io::Read;

use crate::compress::{RCompressData, Result};
use crate::support::BitwiseWrite;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn finalise_compresson197(&mut self) -> Result<()> {
    if !self.uncompressible {
      self.fn207()?;
      self.output_store.finalise()?;
    }
    // self.dat183_IS_CONST_8162 = 0;
    self.array165_counter = 0;
    Ok(())
  }
  pub fn write_stored_bits_to_buffer(&mut self, arg203: i16) -> Result<()> {
    /*
    `arg203` appears to be the bits in the file most of the time
    */
    let i: usize = arg203.try_into().unwrap();
    self
      .output_store
      .write_bits(u32::from(self.dat_arr192[i]), self.dat_arr180[i])?;
    Ok(())
  }
}
