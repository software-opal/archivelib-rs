use crate::compress::{RCompressData, Result};
use crate::support::BitwiseWrite;
use std::io::Read;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn write_run_offset_value_to_file(&mut self, var204: u16) -> Result<()> {
    pure_fn224(
      var204,
      &mut self.output_store,
      &self.bit_length_huff_bit_length,
      &self.bit_length_huffman_encoding,
    )
  }
}

pub fn pure_fn224<W>(var204: u16, out: &mut W, arr181: &[u8], arr194: &[u16]) -> Result<()>
where
  W: BitwiseWrite,
{
  // Calculate the number of bits needed to represent var204.
  let byte_or_run_length203 = 16 - (var204.leading_zeros() as usize);
  out.write_bits(
    arr194[byte_or_run_length203],
    arr181[byte_or_run_length203] as usize,
  )?;
  if byte_or_run_length203 > 1 {
    out.write_bits(var204, byte_or_run_length203 - 1)?;
  }
  Ok(())
}

#[cfg(test)]
mod tests {
  use super::*;
  use crate::support::ExpectedCallWriter;

  #[test]
  fn test_fn224_0() {
    let dat_arr181 = vec![6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let dat_arr194 = vec![
      62, 0, 1, 14, 63, 30, 2, 3, 4, 5, 6, 34912, 34913, 34914, 34915, 34918, 34919, 34920, 34921,
    ];
    let mut expected_calls = ExpectedCallWriter::from_vec(vec![(4, 3), (194, 7)]);
    pure_fn224(194, &mut expected_calls, &dat_arr181, &dat_arr194).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn224_1() {
    let dat_arr181 = vec![6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let dat_arr194 = vec![
      62, 0, 1, 14, 63, 30, 2, 3, 4, 5, 6, 34912, 34913, 34914, 34915, 34918, 34919, 34920, 34921,
    ];
    let mut expected_calls = ExpectedCallWriter::from_vec(vec![(30, 5), (25, 4)]);
    pure_fn224(25, &mut expected_calls, &dat_arr181, &dat_arr194).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn224_2() {
    let dat_arr181 = vec![6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let dat_arr194 = vec![
      62, 0, 1, 14, 63, 30, 2, 3, 4, 5, 6, 34912, 34913, 34914, 34915, 34918, 34919, 34920, 34921,
    ];
    let mut expected_calls = ExpectedCallWriter::from_vec(vec![(2, 3), (41, 5)]);
    pure_fn224(41, &mut expected_calls, &dat_arr181, &dat_arr194).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn224_3() {
    let dat_arr181 = vec![6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let dat_arr194 = vec![
      62, 0, 1, 14, 63, 30, 2, 3, 4, 5, 6, 34912, 34913, 34914, 34915, 34918, 34919, 34920, 34921,
    ];
    let mut expected_calls = ExpectedCallWriter::from_vec(vec![(2, 3), (36, 5)]);
    pure_fn224(36, &mut expected_calls, &dat_arr181, &dat_arr194).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn224_4() {
    let dat_arr181 = vec![6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let dat_arr194 = vec![
      62, 0, 1, 14, 63, 30, 2, 3, 4, 5, 6, 34912, 34913, 34914, 34915, 34918, 34919, 34920, 34921,
    ];
    let mut expected_calls = ExpectedCallWriter::from_vec(vec![(6, 3), (636, 9)]);
    pure_fn224(636, &mut expected_calls, &dat_arr181, &dat_arr194).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn224_5() {
    let dat_arr181 = vec![6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let dat_arr194 = vec![
      62, 0, 1, 14, 63, 30, 2, 3, 4, 5, 6, 34912, 34913, 34914, 34915, 34918, 34919, 34920, 34921,
    ];
    let mut expected_calls = ExpectedCallWriter::from_vec(vec![(5, 3), (442, 8)]);
    pure_fn224(442, &mut expected_calls, &dat_arr181, &dat_arr194).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn224_6() {
    let dat_arr181 = vec![6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let dat_arr194 = vec![
      62, 0, 1, 14, 63, 30, 2, 3, 4, 5, 6, 34912, 34913, 34914, 34915, 34918, 34919, 34920, 34921,
    ];
    let mut expected_calls = ExpectedCallWriter::from_vec(vec![(2, 3), (52, 5)]);
    pure_fn224(52, &mut expected_calls, &dat_arr181, &dat_arr194).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn224_7() {
    let dat_arr181 = vec![6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let dat_arr194 = vec![
      62, 0, 1, 14, 63, 30, 2, 3, 4, 5, 6, 34912, 34913, 34914, 34915, 34918, 34919, 34920, 34921,
    ];
    let mut expected_calls = ExpectedCallWriter::from_vec(vec![(3, 3), (94, 6)]);
    pure_fn224(94, &mut expected_calls, &dat_arr181, &dat_arr194).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn224_8() {
    let dat_arr181 = vec![6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let dat_arr194 = vec![
      62, 0, 1, 14, 63, 30, 2, 3, 4, 5, 6, 34912, 34913, 34914, 34915, 34918, 34919, 34920, 34921,
    ];
    let mut expected_calls = ExpectedCallWriter::from_vec(vec![(3, 3), (65, 6)]);
    pure_fn224(65, &mut expected_calls, &dat_arr181, &dat_arr194).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn224_9() {
    let dat_arr181 = vec![6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let dat_arr194 = vec![
      62, 0, 1, 14, 63, 30, 2, 3, 4, 5, 6, 34912, 34913, 34914, 34915, 34918, 34919, 34920, 34921,
    ];
    let mut expected_calls = ExpectedCallWriter::from_vec(vec![(3, 3), (85, 6)]);
    pure_fn224(85, &mut expected_calls, &dat_arr181, &dat_arr194).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn224_10() {
    let dat_arr181 = vec![6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let dat_arr194 = vec![
      62, 0, 1, 14, 63, 30, 2, 3, 4, 5, 6, 34912, 34913, 34914, 34915, 34918, 34919, 34920, 34921,
    ];
    let mut expected_calls = ExpectedCallWriter::from_vec(vec![(14, 4), (7, 2)]);
    pure_fn224(7, &mut expected_calls, &dat_arr181, &dat_arr194).unwrap();
    expected_calls.assert_drained();
  }
}
