#[macro_use]
mod array_alias;
mod bit_iter;
mod bitreader;
mod readahead_reader;
mod reader;
mod writer;

pub use self::array_alias::ArrayAlias;
pub use self::bit_iter::BitwiseIterable;
pub use self::bitreader::{BitRead, BitReader, ExactCallBitReader};
pub use self::readahead_reader::{BitwiseReadAheadRead, BitwiseReadAheadReader};
pub use self::reader::{BitwiseRead, BitwiseReader, ReadError, VecReader};
pub use self::writer::{BitwiseWrite, BitwiseWriter, ExactCallWriter};

pub fn get_bitmask(bits: usize) -> u128 {
  if bits == 128 {
    u128::max_value()
  } else {
    (1u128 << bits) - 1
  }
}
