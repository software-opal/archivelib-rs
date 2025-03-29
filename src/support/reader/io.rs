use std::collections::VecDeque;

use crate::support::bit_utils::to_bits;

use super::BitwiseRead;

pub struct BitwiseReader<R: std::io::Read> {
  inner: R,
  buffer: VecDeque<bool>,
}

impl<R: std::io::Read> BitwiseReader<R> {
  pub fn new(w: R) -> Self {
    Self {
      inner: w,
      buffer: VecDeque::with_capacity(8),
    }
  }

  pub fn fill_buffer(&mut self) -> std::io::Result<()> {
    if self.buffer.is_empty() {
      let mut buf = [0];
      if self.inner.read(&mut buf)? == 1 {
        self.buffer.extend(to_bits(buf[0].into(), 8))
      }
    }
    Ok(())
  }
}

impl<R: std::io::Read> BitwiseRead for BitwiseReader<R> {
  fn read_bit_or_eof(&mut self) -> std::io::Result<Option<bool>> {
    self.fill_buffer()?;
    Ok(self.buffer.pop_front())
  }
}
