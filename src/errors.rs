#[allow(dead_code)]
#[derive(Fail, Debug)]
pub enum CompressError {
  #[fail(display = "Illegal Compression level: {}", _0)]
  IllegalCompressionLevel(u8),
  #[fail(display = "Uncompressable")]
  InputUncompressable,
  #[fail(display = "Cursor {} Invariant failed", _0)]
  InvalidCursor(String),
  #[fail(display = "Invalid conversion: {}", error)]
  InvalidIntegerConversion {
    #[cause]
    error: std::num::TryFromIntError,
  },
  #[fail(display = "IOError: {}", error)]
  IOError {
    #[cause]
    error: std::io::Error,
  },
}
impl From<std::num::TryFromIntError> for CompressError {
  fn from(v: std::num::TryFromIntError) -> Self {
    Self::InvalidIntegerConversion { error: v }
  }
}
impl From<std::io::Error> for CompressError {
  fn from(v: std::io::Error) -> Self {
    Self::IOError { error: v }
  }
}

#[derive(Debug)]
pub enum BinaryTreeInvariantError {
  Type1,
  Type2,
}

#[derive(Fail, Debug)]
pub enum DecompressError {
  #[fail(display = "Illegal Compression level: {}", _0)]
  IllegalCompressionLevel(u8),
  #[fail(display = "Binary tree error: {:?}", _0)]
  BinaryTreeError(BinaryTreeInvariantError),
  #[fail(display = "Invariant Failure")]
  InvariantFailure,

  #[fail(display = "Internal Error: {}", _0)]
  InternalError(u8),
  // #[fail(display = "Unexpected EoF")]
  // UnexpectedEndOfFile {
  //   #[cause]
  //   error: ReadError,
  // },
  #[fail(display = "Invalid conversion: {}", _0)]
  InvalidIntegerConversion(#[cause] std::num::TryFromIntError),
  #[fail(display = "IOError: {}", _0)]
  IOError(#[cause] std::io::Error),
}

impl From<std::num::TryFromIntError> for DecompressError {
  fn from(v: std::num::TryFromIntError) -> Self {
    DecompressError::InvalidIntegerConversion(v)
  }
}
impl From<std::io::Error> for DecompressError {
  fn from(v: std::io::Error) -> Self {
    DecompressError::IOError(v)
  }
}
