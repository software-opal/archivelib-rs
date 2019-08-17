use std::convert::{TryFrom};
use std::io::Read;

use crate::compress::base::{CompressU16ArrayAlias, CompressU8ArrayAlias};
use crate::compress::{CompressError, RCompressData, Result};
use crate::consts::{
  CONST_N141_IS_511, CONST_N142_IS_15, CONST_N143_IS_9, CONST_N145_IS_19, CONST_N147_IS_5,
  CONST_N540_IS_5,
};
use crate::support::BitwiseWrite;
const CHAR_BIT: usize = 8;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn207(&mut self) -> Result<()> {
    let mut var456: u32 = 0 as u32;
    let mut var217 = [0; 2 * CONST_N145_IS_19 - 1];
    let mut var229 = self.fn211(
      i32::try_from(CONST_N141_IS_511).unwrap(),
      &mut CompressU16ArrayAlias::Array191(0),
      &mut CompressU8ArrayAlias::Array180(0),
      &mut CompressU16ArrayAlias::Array192(0),
    )?;
    let var455 = self.dat_arr191[usize::try_from(var229).unwrap()];
    self.output_store.write_bits(u32::from(var455), 16)?;
    if var229 >= u32::try_from(CONST_N141_IS_511).unwrap() {
      self.fn216(&mut var217);
      var229 = self.fn211(
        i32::try_from(CONST_N145_IS_19).unwrap(),
        &mut CompressU16ArrayAlias::Custom(0, &mut var217),
        &mut CompressU8ArrayAlias::Array181(0),
        &mut CompressU16ArrayAlias::Array194(0),
      )?;
      if var229 >= u32::try_from(CONST_N145_IS_19).unwrap() {
        self.fn218(
          i16::try_from(CONST_N145_IS_19).unwrap(),
          i16::try_from(CONST_N147_IS_5).unwrap(),
          3 as i16,
        )?;
      } else {
        self
          .output_store
          .write_bits(0_u8, usize::try_from(CONST_N147_IS_5).unwrap())?;
        self.output_store.write_bits(
          u32::try_from(var229).unwrap(),
          usize::try_from(CONST_N147_IS_5).unwrap(),
        )?;
      }
      self.fn222()?;
    } else {
      self
        .output_store
        .write_bits(0_u8, usize::try_from(CONST_N147_IS_5).unwrap())?;
      self
        .output_store
        .write_bits(0_u8, usize::try_from(CONST_N147_IS_5).unwrap())?;
      self
        .output_store
        .write_bits(0_u8, usize::try_from(CONST_N143_IS_9).unwrap())?;
      self.output_store.write_bits(
        u32::try_from(var229).unwrap(),
        usize::try_from(CONST_N143_IS_9).unwrap(),
      )?;
    }
    var229 = self.fn211(
      i32::try_from(CONST_N142_IS_15).unwrap(),
      &mut CompressU16ArrayAlias::Array193(0),
      &mut CompressU8ArrayAlias::Array181(0),
      &mut CompressU16ArrayAlias::Array194(0),
    )?;
    if var229 >= u32::try_from(CONST_N142_IS_15).unwrap() {
      self.fn218(
        i16::try_from(CONST_N142_IS_15).unwrap(),
        i16::try_from(CONST_N540_IS_5).unwrap(),
        -1 as i16,
      )?;
    } else {
      self
        .output_store
        .write_bits(0_u8, usize::try_from(CONST_N540_IS_5).unwrap())?;
      self.output_store.write_bits(
        u32::try_from(var229).unwrap(),
        usize::try_from(CONST_N540_IS_5).unwrap(),
      )?;
    }
    let mut var454 = 0 as u32;
    for run_start226 in 0..var455 {
      if run_start226 % 8 == 0 {
        var456 = u32::from(self.dat_arr165[usize::try_from(var454).unwrap()]);
        var454 += 1;
      } else {
        var456 <<= 1;
      }
      if 0 == (var456 & (1 << (CHAR_BIT - 1))) {
        let a1 = i16::from(self.dat_arr165[usize::try_from(var454).unwrap()]);
        self.write_stored_bits_to_buffer(a1)?;
        var454 += 1;
      } else {
        let val = self.dat_arr165[usize::try_from(var454).unwrap()];
        self.write_stored_bits_to_buffer(i16::from(val).wrapping_add(1 << CHAR_BIT))?;
        var454 += 1;
        let var289 = u32::from(self.dat_arr165[usize::try_from(var454).unwrap()])
          + (u32::from(self.dat_arr165[(var454 + 1) as usize]) << 8);
        var454 += 2;
        self.fn224(u16::try_from(var289).unwrap())?;
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
