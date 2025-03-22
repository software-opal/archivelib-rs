use std::fmt::Debug;



pub trait BitwiseRead {
  fn read_bytes<B, L>(&mut self, bit_count: usize) -> std::io::Result<u16>
  where
    B: TryInto<u16> + Debug + Copy,
    L: TryInto<u8> + Debug + Copy;
  fn finalise(&mut self) -> std::io::Result<()>;

  /// Largest number of bits that can be written in a single operation.
  ///
  /// Must match the size of `read_bytes`'s `bits` argument.
  fn max_bit_count(&self) -> u8 {
    u16::BITS as u8
  }
}
