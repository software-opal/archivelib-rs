use std::io;

pub trait BitwiseWrite: io::Write {}

pub struct BitwiseWriter<W: io::Write> {
  inner: W,
}

impl<W: io::Write> BitwiseWriter<W> {
  pub fn new(w: W) -> Self {
    BitwiseWriter { inner: w }
  }
  pub fn into_inner(self) -> W {
    return self.inner;
  }
}

impl<W: io::Write> BitwiseWrite for BitwiseWriter<W> {}
impl<W: io::Write> io::Write for BitwiseWriter<W> {
  fn write(&mut self, buf: &[u8]) -> io::Result<usize> {
    self.inner.write(buf)
  }
  fn flush(&mut self) -> io::Result<()> {
    self.inner.flush()
  }
}
