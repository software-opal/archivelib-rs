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

impl<T: BitwiseWrite> BitwiseWrite for &mut T {
  fn finalise(&mut self) -> std::io::Result<()> {
    (*self).finalise()
  }
  fn write_bits(&mut self, bits: u16, bit_count: usize) -> std::io::Result<()> {
    (*self).write_bits(bits, bit_count)
  }
}
