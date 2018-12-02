use crate::compress::{CompressError, RCompressData, Result};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn finalise_compresson197(&mut self) -> Result<()> {
    if !self.uncompressible {
      self.fn207()?;
    }
    self.finalize_buffer206()?;
    self.dat183_IS_CONST_8162 = 0;
    self.array165_counter = 0;
    Ok(())
  }
  pub fn finalize_buffer206(&mut self) -> Result<()> {
    if !self.uncompressible {
      // Write enough bits to clear out any that have been set, without writing a
      // whole new byte if if no bits need clearing
      self.write_bits_to_buffer(8 - 1, 0)?;
      if 0 != self.buffer_position {
        // Flush the data that is waiting
        self.flush_to_output()?;
      }
    }
    self.buffer_position = 0;
    Ok(())
  }
  pub fn write_bits_to_buffer(&mut self, bit_count209: u16, mut bits203: u16) -> Result<()> {
    /*

    `bit_count209`: Number of bits to use from `bits203`

    `data->bits_buffer_used172` Number of bits in use in `data->bits_buffer182`.
    */
    // Move the assigned bits into the highest bits of `bits203`
    bits203 = ((bits203) << 16 - bit_count209) as u16;
    // Combine the existing bits with these new bits without overlap
    self.bits_buffer182 =
      (self.bits_buffer182 | (bits203 >> self.bits_buffer_used172) as u16) as u16;
    self.bits_buffer_used172 = self.bits_buffer_used172 + bit_count209;
    if self.bits_buffer_used172 >= 8 {
      // Highest 8 bits are assigned(at least); save them to the buffer.
      if self.buffer_position >= 512 {
        self.flush_to_output()?;
      }
      // Take the high bits of bits_buffer182
      self.buffer[self.buffer_position] = (self.bits_buffer182 >> 8) as u8;
      self.buffer_position += 1;
      self.bits_buffer_used172 = (self.bits_buffer_used172 - 8) as u16;
      if (self.bits_buffer_used172) < 8 {
        // Missing enough bits to do the same thing again.
        // Move the low bits of `data->bits_buffer182` into the high bits.
        self.bits_buffer182 = ((self.bits_buffer182) << 8) as u16
      } else {
        if self.buffer_position >= 512 {
          self.flush_to_output()?;
        }
        // Take the low bits of bits_buffer182
        self.buffer[self.buffer_position] = self.bits_buffer182 as u8;
        self.buffer_position += 1;
        self.bits_buffer_used172 = (self.bits_buffer_used172 - 8) as u16;
        // Handle any bits that didn't fit the first time we tried.
        self.bits_buffer182 = ((bits203) << bit_count209 - self.bits_buffer_used172) as u16;
      }
    }
    Ok(())
  }
  pub fn write_stored_bits_to_buffer(&mut self, arg203: i16) -> Result<()> {
    /*
    `arg203` appears to be the bits in the file most of the time
    */
    let a1 = self.dat_arr180[arg203 as usize] as u16;
    let a2 = self.dat_arr192[arg203 as usize];
    self.write_bits_to_buffer(a1, a2)
  }
  pub fn flush_to_output(&mut self) -> Result<()> {
    if self.buffer_position > 0 {
      self.chars_written = self.chars_written + self.buffer_position;
      if self.fail_uncompressible && self.chars_written >= self.input_length {
        self.uncompressible = true;
        return Err(CompressError::InputUncompressable);
      } else {
        self
          .output_store
          .write_all(&self.buffer[..self.buffer_position])?;
        for i in &mut self.buffer[..self.buffer_position] {
          *i = 0
        }
      }
      self.buffer_position = 0;
    }
    Ok(())
  }
}
