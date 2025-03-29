use std::io::Write;

pub struct MatchingWriter<'a> {
  expected: &'a [u8],
  byte_offset: usize,
}

impl<'a> MatchingWriter<'a> {
  pub fn new(input: &'a [u8]) -> Self {
    Self {
      expected: input,
      byte_offset: 0,
    }
  }
  pub fn assert_complete(&self) {
    assert_eq!(
      self.byte_offset,
      self.expected.len(),
      "Write did not complete correctly"
    );
  }
}

impl<'a> Write for MatchingWriter<'a> {
  fn write(&mut self, buf: &[u8]) -> std::io::Result<usize> {
    buf.into_iter().enumerate().for_each(|(idx, byte)| {
      if self.byte_offset >= self.expected.len() {
        panic!(
          "Writing beyond the bounds of the expected data of length {}. Mismatch was at byte {} of a {} byte write.",
           self.expected.len(), idx, buf.len());
      }
      let expected_byte = self.expected[self.byte_offset];
      if *byte != expected_byte {
        panic!("Incorrect byte written(got {:#04X}, expected {:#04X}) at byte {:#X}({}). The mismatch was at byte {} of a {} byte write.", byte, expected_byte, self.byte_offset, self.byte_offset, idx, buf.len());
      }
      self.byte_offset += 1;
    });
    Ok(buf.len())
  }
  fn flush(&mut self) -> std::io::Result<()> {
    Ok(())
  }
}
