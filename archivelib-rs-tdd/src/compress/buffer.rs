use crate::compress::{CompressError, RCompressData, Result};
use crate::support::BitwiseWrite;
use std::io::Read;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn finalise_compresson197(&mut self) -> Result<()> {
    if !self.uncompressible {
      self.fn207()?;
      let bits_remaining = self.writer.write_bits(0, 0)?;
      if bits_remaining > 0 {
        // Fill the gap in the last bit with zeros.
        self.writer.write_bits(0, 8 - bits_remaining)?;
      }
    }
    self.dat183_IS_CONST_8162 = 0;
    self.array165_counter = 0;
    Ok(())
  }
  pub fn write_bits_to_buffer(&mut self, bit_count: u16, mut bits: u16) -> Result<()> {
    self.writer.write_bits(bits.into(), bit_count as usize)
  }
  pub fn write_stored_bits_to_buffer(&mut self, arg203: i16) -> Result<()> {
    /*
    `arg203` appears to be the bits in the file most of the time
    */
    let i = arg203 as usize;
    self
      .writer
      .write_bits(self.dat_arr180[i], self.dat_arr192[i] as usize)
  }
}
