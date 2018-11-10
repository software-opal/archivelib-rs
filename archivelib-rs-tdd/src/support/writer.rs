use std::fmt;
use std::io;

pub struct Writer {
  data: Vec<u8>,
}

impl Writer {
  pub fn new() -> Self {
    Writer {
      data: Vec::with_capacity(512),
    }
  }

  pub fn into_data(self) -> Box<[u8]> {
    return self.data.into_boxed_slice();
  }

  pub fn write_byte(&mut self, byte: u8) -> io::Result<()> {
    self.data.push(byte);
    Ok(())
  }
}

impl fmt::Debug for Writer {
  fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
    write!(f, "Writer {{ data: {:?} }}", self.data)
  }
}
