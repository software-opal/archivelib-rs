mod base;
mod io;

pub use base::BitwiseRead;
pub use io::BitwiseReader;

#[cfg(test)]
mod bits;
#[cfg(test)]
pub use bits::BitBasedBitwiseReader;
