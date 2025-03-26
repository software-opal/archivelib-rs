use crate::support::bit_iter::ToBits;

pub fn truncate_bits(bits: u16, len: usize) -> u16 {
  bits & (0xFFFF >> (16 - len))
}

/// Iterates from highest bit to lowest bit
pub fn to_bits(bits: u16, bit_count: usize) -> impl Iterator<Item = bool> {
  bits
    .to_bits()
    .into_iter()
    .skip((u16::BITS as usize) - bit_count)
}
pub trait BitwiseWrite {
  fn write_bits(&mut self, bits: u16, bit_count: usize) -> std::io::Result<()>;
  fn finalise(&mut self) -> std::io::Result<()>;

  /// Largest number of bits that can be written in a single operation.
  ///
  /// Must match the size of `write_bits`'s `bits` argument.
  fn max_bit_count(&self) -> usize {
    u16::BITS as usize
  }
}
