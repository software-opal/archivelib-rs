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
}
