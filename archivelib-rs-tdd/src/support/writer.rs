use std::io;

pub trait BitwiseWrite {}

pub struct BitwiseWriter {}

impl BitwiseWriter {
  pub fn new() -> Self {
    unimplemented!();
    // Writer {
    //   data: Vec::with_capacity(512),
    // }
  }

  pub fn into_data(self) -> Box<[u8]> {
    unimplemented!();
    // return self.data.into_boxed_slice();
  }

  pub fn write_byte(&mut self, byte: u8) -> io::Result<()> {
    unimplemented!();
    // self.data.push(byte);
    // Ok(())
  }
}

impl BitwiseWrite for BitwiseWriter {}
