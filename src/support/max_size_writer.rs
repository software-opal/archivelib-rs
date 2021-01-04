use std::io;

pub struct MaxSizeWriter<W: io::Write> {
  writer: W,
  max_size: usize,
  written: usize,
}

impl<W: io::Write> MaxSizeWriter<W> {
  pub fn wrap(writer: W, max_size: usize) -> Self {
    MaxSizeWriter {
      writer,
      max_size,
      written: 0,
    }
  }
  pub fn into_inner(self) -> W {
    self.writer
  }
}

impl<W: io::Write> io::Write for MaxSizeWriter<W> {
  fn write(&mut self, buf: &[u8]) -> io::Result<usize> {
    if self.written >= self.max_size {
      return Ok(0);
    }
    let write_limit = self.max_size - self.written;
    let to_write = write_limit.min(buf.len());
    let written = self.writer.write(&buf[..to_write])?;
    assert!(written <= to_write);
    self.written += written;
     Ok(written)
  }
  fn flush(&mut self) -> io::Result<()> {
    self.writer.flush()
  }
}

#[cfg(test)]
mod tests {
  use super::*;
  use std::io::Write;

  #[test]
  fn test_writer_prevents_bigger_writes() {
    let out = vec![];
    let mut writer = MaxSizeWriter::wrap(out, 10);
    assert_eq!(writer.write(&[0, 1, 2, 3, 4, 5, 6, 7]).unwrap(), 8);
    assert_eq!(writer.write(&[0, 1, 2, 3, 4, 5, 6, 7]).unwrap(), 2);
    assert_eq!(writer.into_inner(), vec![0, 1, 2, 3, 4, 5, 6, 7, 0, 1])
  }
}
