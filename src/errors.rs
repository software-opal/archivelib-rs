use thiserror::Error;

#[derive(Error, Debug)]
pub enum CompressError {
  #[error("Illegal Compression level: {0}")]
  IllegalCompressionLevel(u8),
  #[error("Uncompressable")]
  InputUncompressable,
  #[error("Cursor {0} Invariant failed")]
  InvalidCursor(String),
  #[error("Invalid conversion: {error}")]
  InvalidIntegerConversion {
    #[from]
    error: std::num::TryFromIntError,
  },
  #[error("IOError: {error}")]
  IOError {
    #[from]
    error: std::io::Error,
  },
}

#[derive(Debug)]
pub enum BinaryTreeInvariantError {
  Type1,
  Type2,
}

#[derive(Error, Debug)]
pub enum DecompressError {
  #[error("Illegal Compression level: {0}")]
  IllegalCompressionLevel(u8),
  #[error("Binary tree error: {0:?}")]
  BinaryTreeError(BinaryTreeInvariantError),
  #[error("Invariant Failure")]
  InvariantFailure,

  #[error("Internal Error: {0}")]
  InternalError(u8),
  // #[error("Unexpected EoF")]
  // UnexpectedEndOfFile {
  //   #[cause]
  //   error: ReadError,
  // },
  #[error("Invalid conversion: {0}")]
  InvalidIntegerConversion(#[from] std::num::TryFromIntError),
  #[error("IOError: {0}")]
  IOError(#[from] std::io::Error),
}
