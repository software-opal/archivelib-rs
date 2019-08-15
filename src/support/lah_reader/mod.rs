mod base;
mod correct;

pub use self::base::{LookAheadBitwiseRead, LookAheadBitwiseReader};
pub use self::correct::{CorrectLookAheadBitwiseRead, CorrectLookAheadBitwiseReader};

#[cfg(test)]
pub use self::base::ExpectedCallLookAheadBitwiseReader;
