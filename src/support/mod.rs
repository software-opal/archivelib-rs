#[macro_use]
mod array_alias;
mod bit_iter;
mod bitreader;
mod lookahead_reader;
mod reader;
mod writer;

pub use self::array_alias::ArrayAlias;
pub use self::bit_iter::{FromBits, IntoBits};
pub use self::bitreader::{BitRead, BitReader, ExactCallBitReader};
pub use self::lookahead_reader::{LookAheadBitwiseRead, LookAheadBitwiseReader};
pub use self::reader::{BitwiseRead, BitwiseReader, ReadError, VecReader};
pub use self::writer::{BitwiseWrite, BitwiseWriter, ExactCallWriter, NullBitwiseWriter};

pub fn get_bitmask(bits: usize) -> u128 {
  if bits == 128 {
    u128::max_value()
  } else {
    (1u128 << bits) - 1
  }
}
