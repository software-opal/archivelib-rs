use std::convert::TryInto;
use std::io::Read;

use crate::compress::{RCompressData, Result};
use crate::support::BitwiseWrite;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  /// Obfuscated name: `void _197()`
  pub fn finalise_compresson(&mut self) -> Result<()> {
    if !self.uncompressible {
      self.fn207_maybe_flush_state_to_output()?;
      self.output_store.finalise()?;
    }
    // self.dat183_IS_CONST_8162 = 0;
    self.byte_or_run_buffer_index = 0;
    Ok(())
  }
  pub fn write_stored_bits_to_buffer(&mut self, arg203: i16) -> Result<()> {
    /*
    `arg203` appears to be the bits in the file most of the time
    */
    let i: usize = arg203.try_into().unwrap();
    self.output_store.write_bits(
      u32::from(self.byte_run_length_huff_encoding[i]),
      self.byte_run_length_huff_bit_length[i],
    )?;
    Ok(())
  }
}
