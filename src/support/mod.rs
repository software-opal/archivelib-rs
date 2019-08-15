#[macro_use]
mod bit_iter;
mod writer;

pub use self::writer::{BitwiseWrite, BitwiseWriter, ExactCallWriter, NullBitwiseWriter};

// #[cfg(feature = "new_impl")]
mod lah_reader;
#[cfg(feature = "new_impl")]
pub use self::lah_reader::*;

#[cfg(not(feature = "new_impl"))]
mod bitreader;
#[cfg(all(test, not(feature = "new_impl")))]
pub use self::bitreader::ExactCallBitReader;
#[cfg(not(feature = "new_impl"))]
pub use self::bitreader::{BitRead, BitReader};

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
