use crate::support::ReadError;

#[derive(Fail, Debug, PartialEq)]
pub enum DataError {
  #[fail(display = "Read error: {:?}", _0)]
  ReadError(#[cause] ReadError),
  #[fail(display = "Input data did not meet required invariants. Code: {}", _0)]
  InvariantFailed(u8),
}

impl From<ReadError> for DataError {
  fn from(err: ReadError) -> DataError {
    DataError::ReadError(err)
  }
}

pub struct LookupTable {
  dat_arr180: Vec<u8>,
  dat_arr181: Vec<u8>,
  dat_arr189: Vec<u16>,
  dat_arr190: Vec<u16>,
  dat_arr240: Vec<u16>,
  dat_arr241: Vec<u16>,
}

impl LookupTable {}
