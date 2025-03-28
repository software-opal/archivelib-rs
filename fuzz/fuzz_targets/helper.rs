use std::io::Write;

macro_rules! fuzz_with_main {
  ($fn: ident) => {
    #[cfg(fuzzing)]
    #[macro_use]
    extern crate libfuzzer_sys;

    #[cfg(fuzzing)]
    fuzz_target!(|data: &[u8]| {
      $fn(data);
    });

    #[cfg(not(fuzzing))]
    fn main() {
      use std::fs::File;
      use std::io::Read;

      let mut buffer = vec![];
      File::open(std::env::args().nth(1).unwrap())
        .unwrap()
        .read_to_end(&mut buffer)
        .unwrap();
      target(&buffer);
    }
  };
  (|$data:ident: &[u8]| $body:expr) => {
    fn target($data: &[u8]) {
      $body
    }
    fuzz_with_main!(target);
  };
}

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
        panic!("Incorrect bit written at byte {}. The mismatch was at byte {} of a {} byte write.", self.byte_offset, idx, buf.len());
      }
      self.byte_offset += 1;
    });
    Ok(buf.len())
  }
  fn flush(&mut self) -> std::io::Result<()> {
    Ok(())
  }
}
