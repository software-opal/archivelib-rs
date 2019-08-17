use std::convert::TryInto;
use std::fmt::Debug;

use super::base::BitwiseWrite;

pub struct ExpectedCallWriter {
  calls: Vec<(u128, usize)>,
  write_calls: usize,
  pub written_bits: usize,
}

impl ExpectedCallWriter {
  pub fn from_vec(calls: Vec<(u128, usize)>) -> Self {
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
  fn write_bits<B, L>(&mut self, bits: B, bit_count: L) -> std::io::Result<usize>
  where
    B: TryInto<u128> + Debug + Copy,
    L: TryInto<usize> + Debug + Copy,
  {
    let bits = bits
      .try_into()
      .map_err(|_| format!("Cannot convert bits({:#X?}) to u128", bits))
      .unwrap();
    let bit_count = bit_count
      .try_into()
      .map_err(|_| format!("Cannot convert bit_count({:#X?}) to usize", bits))
      .unwrap();
    assert!(
      !self.calls.is_empty(),
      "Attempting to call write_bits({:X}, {}) when it wasn't expected; Calls expended",
      bits,
      bit_count
    );
    let actual = (bits, bit_count);
    let expected = self.calls.remove(0);
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
    Ok(self.written_bits % 8)
  }
  fn finalise(&mut self) -> std::io::Result<()> {
    let expected = self.calls.remove(0);
    assert_eq!(self.calls, vec![]);
    assert_eq!(expected.0, 0);
    Ok(())
  }
}
