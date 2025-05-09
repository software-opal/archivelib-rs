use thiserror::Error;

#[derive(Error, Debug)]
pub enum DecompressError {
  #[error("Invalid binary tree")]
  InvalidBinaryTree,
  #[error("Invalid run length: {0}")]
  InvalidRunLength(usize),
  #[error("Invalid run offset: {0}")]
  InvalidRunOffset(usize),
  #[error("IOError: {error}")]
  IOError {
    #[from]
    error: std::io::Error,
  },
}
