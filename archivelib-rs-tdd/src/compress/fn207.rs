use crate::compress::base::{CompressU16ArrayAlias, CompressU8ArrayAlias};
use crate::compress::{CompressError, RCompressData, Result};
use crate::consts::{
  CONST_N141_IS_511, CONST_N142_IS_15, CONST_N143_IS_9, CONST_N145_IS_19, CONST_N147_IS_5,
  CONST_N540_IS_5,
};
use crate::support::BitwiseWrite;
use std::io::Read;
const CHAR_BIT: usize = 8;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn207(&mut self) -> Result<()> {
    let mut var456: u32 = 0 as u32;
    let mut var217 = [0; 2 * CONST_N145_IS_19 - 1];
    let mut var229 = self.fn211(
      CONST_N141_IS_511 as i32,
      &mut CompressU16ArrayAlias::Array191(0),
      &mut CompressU8ArrayAlias::Array180(0),
      &mut CompressU16ArrayAlias::Array192(0),
    )?;
    let var455 = self.dat_arr191[var229 as usize];
    self.output_store.write_bits(var455 as u32, 16 as usize)?;
    if var229 >= CONST_N141_IS_511 as u32 {
      self.fn216(&mut var217);
      var229 = self.fn211(
        CONST_N145_IS_19 as i32,
        &mut CompressU16ArrayAlias::Custom(0, &mut var217),
        &mut CompressU8ArrayAlias::Array181(0),
        &mut CompressU16ArrayAlias::Array194(0),
      )?;
      if var229 >= CONST_N145_IS_19 as u32 {
        self.fn218(CONST_N145_IS_19 as i16, CONST_N147_IS_5 as i16, 3 as i16)?;
      } else {
        self
          .output_store
          .write_bits(0u8, CONST_N147_IS_5 as usize)?;
        self
          .output_store
          .write_bits(var229 as u32, CONST_N147_IS_5 as usize)?;
      }
      self.fn222()?;
    } else {
      self
        .output_store
        .write_bits(0u8, CONST_N147_IS_5 as usize)?;
      self
        .output_store
        .write_bits(0u8, CONST_N147_IS_5 as usize)?;
      self
        .output_store
        .write_bits(0u8, CONST_N143_IS_9 as usize)?;
      self
        .output_store
        .write_bits(var229 as u32, CONST_N143_IS_9 as usize)?;
    }
    var229 = self.fn211(
      CONST_N142_IS_15 as i32,
      &mut CompressU16ArrayAlias::Array193(0),
      &mut CompressU8ArrayAlias::Array181(0),
      &mut CompressU16ArrayAlias::Array194(0),
    )?;
    if var229 >= CONST_N142_IS_15 as u32 {
      self.fn218(CONST_N142_IS_15 as i16, CONST_N540_IS_5 as i16, -1 as i16)?;
    } else {
      self
        .output_store
        .write_bits(0u8, CONST_N540_IS_5 as usize)?;
      self
        .output_store
        .write_bits(var229 as u32, CONST_N540_IS_5 as usize)?;
    }
    let mut var454 = 0 as u32;
    for run_start226 in 0..var455 {
      if run_start226 % 8 == 0 {
        var456 = self.dat_arr165[var454 as usize] as u32;
        var454 += 1;
      } else {
        var456 <<= 1;
      }
      if 0 != (var456 & (1 << (CHAR_BIT - 1))) {
        let val = self.dat_arr165[var454 as usize];
        self.write_stored_bits_to_buffer((val as i16).wrapping_add(1 << CHAR_BIT))?;
        var454 += 1;
        let var289 = self.dat_arr165[var454 as usize] as u32
          + ((self.dat_arr165[(var454 + 1) as usize] as u32) << 8);
        var454 += 2;
        self.fn224(var289 as u16)?;
      } else {
        let a1 = self.dat_arr165[var454 as usize] as i16;
        self.write_stored_bits_to_buffer(a1)?;
        var454 += 1;
      }
      if self.uncompressible {
        return Err(CompressError::InputUncompressable);
      }
    }
    for i in 0..CONST_N141_IS_511 {
      self.dat_arr191[i] = 0 as u16;
    }
    for i in 0..CONST_N142_IS_15 {
      self.dat_arr193[i] = 0 as u16;
    }
    Ok(())
  }
}
