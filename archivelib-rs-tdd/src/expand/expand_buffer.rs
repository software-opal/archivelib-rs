use crate::expand::{RExpandData, Result};
use crate::support::{BitwiseReadAheadRead, BitwiseWrite};

const CHAR_BIT: i16 = 8;

impl<R: BitwiseReadAheadRead, W: BitwiseWrite> RExpandData<R, W> {
  pub fn read_bits(&mut self, mut bits_to_load219: i16) -> Result<()> {
    assert!(0 < bits_to_load219 && bits_to_load219 <= 16);
    /*
    Reads `bits_to_load219` bits into the LSB side of `data->bits182`.
    */
    while bits_to_load219 as i16 > self.bits_in_buffer172 {
      // This loop loads 1 new byte into `data->tmp_bit_buffer245`(the temporary
      // buffer)
      bits_to_load219 = bits_to_load219 - self.bits_in_buffer172;
      // Rotate in the remaining bits from the tmp_bit_buffer.
      self.bits182 = ((self.bits182) << self.bits_in_buffer172)
        + (self.tmp_bit_buffer245 as u16 >> 8 - self.bits_in_buffer172);
      self.tmp_bit_buffer245 = self.input_store.read_byte()?;
      self.bits_in_buffer172 = CHAR_BIT;
    }
    self.bits_in_buffer172 = self.bits_in_buffer172 - bits_to_load219;
    self.bits182 =
      ((self.bits182) << bits_to_load219) + (self.tmp_bit_buffer245 as u16 >> 8 - bits_to_load219);
    self.tmp_bit_buffer245 = self.tmp_bit_buffer245 << bits_to_load219;
    Ok(())
  }

  pub fn get_bits(&mut self, bits_to_load219: i16) -> Result<u16> {
    let bits: u16 = self.bits182 >> (2 * 8 - bits_to_load219);
    self.read_bits(bits_to_load219)?;
    return Ok(bits);
  }
}
