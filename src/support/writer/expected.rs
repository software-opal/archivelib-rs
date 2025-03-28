use crate::support::bit_utils::truncate_bits;

use super::base::BitwiseWrite;
pub struct ExpectedCallWriter {
  calls: Vec<(u16, usize)>,
  write_calls: usize,
  pub written_bits: usize,
}

impl ExpectedCallWriter {
  pub fn from_vec(calls: Vec<(u16, usize)>) -> Self {
    Self {
      calls,
      write_calls: 0,
      written_bits: 0,
    }
  }
  pub fn assert_drained(&self) {
    assert_eq!(self.calls, vec![])
  }
}
impl BitwiseWrite for ExpectedCallWriter {
  fn write_bits(&mut self, bits: u16, bit_count: usize) -> std::io::Result<()> {
    assert!(
      bit_count <= self.max_bit_count(),
      "Too many bits written at once"
    );

    assert!(
      !self.calls.is_empty(),
      "Attempting to call write_bits({:X}, {}) when it wasn't expected; Calls expended",
      bits,
      bit_count
    );
    let actual = (truncate_bits(bits, bit_count), bit_count);
    let (exp_bits, exp_bit_count) = self.calls.remove(0);
    let expected = (truncate_bits(exp_bits, exp_bit_count), exp_bit_count);
    if self.calls.is_empty() {
      assert_eq!(actual.0, expected.0);
    } else {
      assert_eq!(
        actual, expected,
        "Trying to write {:?} when {:?} was expected at index {}",
        actual, expected, self.write_calls
      );
    }
    self.written_bits += bit_count;
    self.write_calls += 1;
    Ok(())
  }
  fn finalise(&mut self) -> std::io::Result<()> {
    let expected = self.calls.remove(0);
    assert_eq!(self.calls, vec![]);
    assert_eq!(expected.0, 0);
    Ok(())
  }
}
