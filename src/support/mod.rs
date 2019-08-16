#[macro_use]
mod bit_iter;
mod writer;

#[cfg(feature = "new_impl")]
mod lah_reader;
#[cfg(feature = "new_impl")]
pub use self::lah_reader::*;

pub use self::writer::*;

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

#[macro_export]
macro_rules! check_rust_against_sys_decompress {
  ($compressed: ident) => {{
    let compressed = $compressed;
    match crate::do_decompress(&compressed[..]) {
      Ok(decompress_output) => {
        match archivelib_sys::do_decompress(&compressed[..]) {
          Ok(sys_output) => {
            assert_bytes_eq!(&sys_output, &decompress_output[..]);
          }
          Err(err) => {
            assert!(false, "System library failed with error; but rust library succeeded. Error: {}", err);
          }
        }
      }
      Err(msg) => {
        if msg == "BinaryTreeError(Type1)" || msg == "BinaryTreeError(Type2)" {
          match archivelib_sys::do_decompress(&compressed[..]) {
            Ok(_sys_output) => {
              panic!("archivelib::do_decompress failed with a binary tree error({}); but the system library succeeded!", msg)
            }
            Err(_err) => {unimplemented!();}
          }
        } else if msg == "InvariantFailue" {
          // These usually crash the system library; so just *assume* the input is fine
        } else if msg == "FileExhausted" {
          unimplemented!();
        } else {
          panic!("archivelib::do_decompress failed with an unexpected error: {}", msg);
        }
      }
    }
  }};
}
