use crate::consts::*;
use crate::support::{BitwiseRead, BitwiseWrite, ReadError};

pub fn do_decompress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  unimplemented!();
}

#[derive(Debug)]
pub struct ExpandData<R, W> {
  input: R,
  output: W,
  input_length: usize,
}

impl<R: BitwiseRead, W: BitwiseWrite> ExpandData<R, W> {
  pub fn new(input: R, output: W, input_length: usize, compression_level: u8) -> Option<Self> {
    if compression_level > MAX_COMPRESSION_FACTOR || compression_level < MIN_COMPRESSION_FACTOR {
      return None;
    }
    Some(ExpandData {
      input: input,
      output: output,
      input_length: input_length,
    })
  }

  pub fn into_output(self) -> W {
    self.output
  }

  pub fn decompress(&mut self) -> Result<(), ReadError> {
    unimplemented!();
  }
}

#[cfg(test)]
mod test {
  use super::*;
  use crate::support::{BitwiseReader, BitwiseWriter};

  #[test]
  fn test_create_new_expand_data_succeds() {
    let data: &[u8] = &vec![];
    let res = ExpandData::new(
      BitwiseReader::new(data), // input_store
      BitwiseWriter::new(),     // output_store
      0,                        // input_length
      MIN_COMPRESSION_FACTOR,   // compression_level
    )
    .unwrap();
    assert_eq!(res.input_length, 0);
  }
}
