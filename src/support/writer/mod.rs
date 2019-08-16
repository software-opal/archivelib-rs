mod base;

#[cfg(test)]
mod expected;

pub use self::base::{BitwiseWrite, BitwiseWriter};

#[cfg(test)]
pub use self::expected::ExpectedCallWriter;
