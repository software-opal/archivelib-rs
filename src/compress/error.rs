use thiserror::Error;

#[derive(Error, Debug)]
pub enum CompressError {
  #[error("IOError: {error}")]
  IOError {
    #[from]
    error: std::io::Error,
  },
}
