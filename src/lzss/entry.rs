use crate::consts_rewrite::EOF_FLAG;

use super::utils::bit_size;

#[derive(Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum LzssEntry {
  Byte(u8),
  /// (run length, offset)
  /// Offset starts at `0` indicating the run starts at the byte prior.
  Run(usize, usize),
  EoF,
}
impl LzssEntry {
  pub fn byte_size(&self) -> usize {
    match self {
      Self::Byte(_) => 1,
      Self::Run(_, _) => 3,
      Self::EoF => 3,
    }
  }
  pub fn lookup_value(&self) -> usize {
    match self {
      Self::Byte(b) => (*b).into(),
      Self::Run(run, _) => 0x100 + (*run) - 3,
      Self::EoF => EOF_FLAG,
    }
  }
  pub fn offset_value(&self) -> Option<usize> {
    match self {
      Self::Byte(_) => None,
      Self::Run(_, offset) => Some(*offset),
      Self::EoF => Some(0),
    }
  }
  pub fn offset_bit_size(&self) -> Option<usize> {
    self.offset_value().map(bit_size)
  }
}
