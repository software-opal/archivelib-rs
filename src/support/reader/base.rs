pub trait BitwiseRead {
  fn read_bit_or_eof(&mut self) -> std::io::Result<Option<bool>>;
  fn read_bit(&mut self) -> std::io::Result<bool> {
    Ok(self.read_bit_or_eof()?.unwrap_or(false))
  }
  fn read_bits(&mut self, bit_count: usize) -> std::io::Result<u16> {
    assert!(bit_count <= self.max_bit_count());
    let mut out = 0;
    for _ in 0..bit_count {
      out = (out << 1) | u16::from(self.read_bit()?)
    }
    Ok(out)
  }

  fn iter_bits(&mut self) -> BitwiseReadBitIterator<'_, Self>
  where
    Self: Sized,
  {
    BitwiseReadBitIterator::new(self)
  }

  /// Largest number of bits that can be written in a single operation.
  ///
  /// Must match the size of `read_bytes`'s `bits` argument.
  fn max_bit_count(&self) -> usize {
    u16::BITS as usize
  }
}

pub struct BitwiseReadBitIterator<'a, R: BitwiseRead> {
  reader: &'a mut R,
  has_errored: bool,

  error: Option<std::io::Error>,
  error_checked: bool,
}

impl<'a, R: BitwiseRead> BitwiseReadBitIterator<'a, R> {
  fn new(reader: &'a mut R) -> Self {
    Self {
      reader,
      has_errored: false,
      error: None,
      error_checked: false,
    }
  }

  pub fn error(&mut self) -> std::io::Result<()> {
    self.error_checked = true;
    match self.error.take() {
      None => {
        assert!(!self.has_errored, "Attempting to call error on a ");
        Ok(())
      }
      Some(e) => Err(e),
    }
  }
}

impl<R: BitwiseRead> Iterator for BitwiseReadBitIterator<'_, R> {
  type Item = bool;
  fn next(&mut self) -> Option<bool> {
    self.error_checked = false;
    if self.has_errored {
      None
    } else {
      match self.reader.read_bit() {
        Ok(bit) => Some(bit),
        Err(e) => {
          self.error = Some(e);
          self.has_errored = true;
          None
        }
      }
    }
  }
}

impl<R: BitwiseRead> Drop for BitwiseReadBitIterator<'_, R> {
  fn drop(&mut self) {
    assert!(
      self.error_checked,
      "Must check the error state of the bitwise read iterator before it leaves scope."
    )
  }
}
