use super::base::LookAheadBitwiseRead;
use super::correct::CorrectLookAheadBitwiseReader;
use crate::support::bit_iter::ToBits;

pub struct ExpectedCallLookAheadBitwiseReader {
  data: Vec<bool>,
  index: usize,
  consume_calls: Vec<usize>,
  consume_call_index: usize,
}

impl ExpectedCallLookAheadBitwiseReader {
  pub fn new_correct(
    data: impl ToBits,
    consume_calls: &[usize],
  ) -> CorrectLookAheadBitwiseReader<Self> {
    CorrectLookAheadBitwiseReader::new(ExpectedCallLookAheadBitwiseReader {
      data: data.to_bits().into_vec(),
      index: 0,
      consume_calls: consume_calls.to_vec(),
      consume_call_index: 0,
    })
  }
}

impl LookAheadBitwiseRead for ExpectedCallLookAheadBitwiseReader {
  fn consume_bits(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    assert!(
      self.consume_call_index < self.consume_calls.len(),
      "Unexpected consume call for {} bits; Too many calls",
      bits
    );
    let expected = self.consume_calls[self.consume_call_index];
    assert_eq!(
      bits, expected,
      "Unexpected consume call(#{}) for {} bits; was expecting a call for {} bits",
      self.consume_call_index, bits, expected
    );

    let items = self.look_ahead_bits(bits)?;
    self.index += bits;
    self.consume_call_index += 1;
    Ok(items)
  }
  fn look_ahead_bits(&mut self, bits: usize) -> std::io::Result<Vec<bool>> {
    let data = self
      .data
      .iter()
      .skip(self.index)
      .take(bits)
      .cloned()
      .collect();
    Ok(data)
  }
}
