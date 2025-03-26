#[macro_use]
mod int_conv;
#[macro_use]
mod debug;

pub mod binary_tree_printer;
mod bit_iter;
mod max_size_writer;
pub mod reader;
pub mod writer;

#[cfg(feature = "new_impl")]
mod lah_reader;
#[cfg(feature = "new_impl")]
pub use self::lah_reader::*;

pub use self::max_size_writer::*;
pub use self::reader::{BitwiseRead, BitwiseReader};
pub use self::writer::{BitwiseWrite, BitwiseWriter};

#[cfg(not(feature = "new_impl"))]
mod bitreader;
#[cfg(all(test, not(feature = "new_impl")))]
pub use self::bitreader::ExactCallBitReader;
#[cfg(not(feature = "new_impl"))]
pub use self::bitreader::{BitRead, BitReader};
