mod reader;
mod writer;

pub use self::reader::{BitwiseRead, BitwiseReader, ReadError, VecReader};
pub use self::writer::{BitwiseWrite, BitwiseWriter};

pub fn get_bitmask(bits: usize) -> u128 {
  if bits == 128 {
    u128::max_value()
  } else {
    (1u128 << bits) - 1
  }
}
