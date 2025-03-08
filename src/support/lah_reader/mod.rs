mod base;
mod correct;

#[cfg(test)]
mod expected;

pub use self::correct::{CorrectLookAheadBitwiseRead, CorrectLookAheadBitwiseReader};

#[cfg(test)]
pub use self::base::{LookAheadBitwiseRead, LookAheadBitwiseReader};
#[cfg(test)]
pub use self::expected::ExpectedCallLookAheadBitwiseReader;
