#[macro_use]
mod array_alias;
pub mod bit_iter;
mod bitreader;
pub mod lookahead_reader;
mod reader;
mod writer;

pub use self::array_alias::ArrayAlias;
pub use self::bit_iter::{FromBits, ToBits};
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

pub fn get_bit_string<T>(bits: T, size: usize) -> std::string::String
where
  T: Into<u128>,
{
  let mut bitstring = format!("{:b}", bits.into());
  while bitstring.len() < size {
    bitstring.insert(0, '0');
  }
  while bitstring.len() > size {
    bitstring.remove(0);
  }
  return bitstring;
}

macro_rules! pending_test {
  () => ({
    pending_test!("?")
  });
  ($msg:expr) => ({
    #[cfg(any(feature = "fuzz-afl", feature = "fuzz-hfuzz"))]
    {
      eprintln!("{}:{} is pending a test case: {}", file!(), line!(), $msg);
      std::process::abort();
    }
    #[cfg(all(not(feature = "fuzz-afl"), not(feature = "fuzz-hfuzz")))]
    {
      unimplemented!("{}:{} is pending a test case: {}", file!(), line!(), $msg);
    }
  });
  ($msg:expr,) => ({
    pending_test!($msg)
  });
  ($fmt:expr, $($arg:tt)+) => ({
    pending_test!(&format_args!($fmt, $($arg)+))
  });
}

#[cfg(test)]
mod tests {
  use super::*;

  #[test]
  fn test_get_bit_string() {
    assert_eq!(get_bit_string(0b1010_u8, 4), "1010");
    assert_eq!(get_bit_string(0b1010_u8, 8), "00001010");
    assert_eq!(get_bit_string(0b1010_u8, 2), "10");
  }
}
