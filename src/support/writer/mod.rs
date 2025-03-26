mod base;
mod io;

pub use self::base::BitwiseWrite;
pub use self::io::BitwiseWriter;

#[cfg(test)]
mod expected;
#[cfg(test)]
pub use self::expected::ExpectedCallWriter;
