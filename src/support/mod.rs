#[macro_use]
mod int_conv;
#[macro_use]
mod debug;

mod bit_iter;
mod max_size_writer;
mod writer;

#[cfg(feature = "new_impl")]
mod lah_reader;
#[cfg(feature = "new_impl")]
pub use self::lah_reader::*;

pub use self::max_size_writer::*;
pub use self::writer::*;

#[cfg(not(feature = "new_impl"))]
mod bitreader;
#[cfg(all(test, not(feature = "new_impl")))]
pub use self::bitreader::ExactCallBitReader;
#[cfg(not(feature = "new_impl"))]
pub use self::bitreader::{BitRead, BitReader};
