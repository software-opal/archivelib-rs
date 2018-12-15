use crate::compress::{RCompressData, Result};
use crate::support::BitwiseWrite;
use std::io::Read;

const USHRT_MAX: u16 = u16::max_value();

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn218(
    &mut self,
    mut bits_to_write: i16,
    bit_length: i16,
    run_start_check: i16,
  ) -> Result<()> {
    pure_fn218(
      &mut self.output_store,
      &self.dat_arr181,
      bits_to_write as usize,
      bit_length as usize,
      run_start_check as i8,
    )
  }
}

fn pure_fn218<W>(
  out: &mut W,
  arr181: &[u8],
  mut bits_to_write: usize,
  bit_length: usize,
  run_start_check: i8,
) -> Result<()>
where
  W: BitwiseWrite + Sized,
{
  assert!(run_start_check == -1 || run_start_check == 3);

  while bits_to_write > 0 && arr181[bits_to_write - 1] == 0 {
    bits_to_write -= 1
  }
  out.write_bits(bits_to_write, bit_length)?;
  let mut run_start226: usize = 0;
  while run_start226 < bits_to_write {
    let var289 = arr181[run_start226];
    run_start226 = run_start226 + 1;
    if var289 <= 6 {
      out.write_bits(var289, 3)?;
    } else {
      out.write_bits(USHRT_MAX << 1, var289 - 3)?;
    }
    if run_start_check == 3 && run_start226 == 3 {
      while run_start226 < 6 && arr181[run_start226] == 0 {
        run_start226 += 1
      }
      out.write_bits(run_start226 - 3, 2)?;
    }
  }
  Ok(())
}
#[cfg(test)]
mod tests {
  use super::*;
  use crate::support::ExactCallWriter;

  #[test]
  fn test_fn218_0() {
    let dat_arr181 = vec![0, 3, 2, 0, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (7, 5),
      (0, 3),
      (3, 3),
      (2, 3),
      (1, 2),
      (3, 3),
      (2, 3),
      (2, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 19, 5, 3).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_1() {
    let dat_arr181 = vec![3, 4, 2, 4, 0, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (8, 5),
      (3, 3),
      (4, 3),
      (2, 3),
      (0, 2),
      (4, 3),
      (0, 3),
      (3, 3),
      (2, 3),
      (3, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 19, 5, 3).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_2() {
    let dat_arr181 = vec![7, 7, 6, 0, 0, 0, 6, 5, 1, 2, 4, 4, 5, 6, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (14, 5),
      (65534, 4),
      (65534, 4),
      (6, 3),
      (3, 2),
      (6, 3),
      (5, 3),
      (1, 3),
      (2, 3),
      (4, 3),
      (4, 3),
      (5, 3),
      (6, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 19, 5, 3).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_3() {
    let dat_arr181 = vec![2, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls =
      ExactCallWriter::from(vec![(4, 5), (2, 3), (0, 3), (2, 3), (0, 2), (1, 3)]);
    pure_fn218(&mut expected_calls, &dat_arr181, 19, 5, 3).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_4() {
    let dat_arr181 = vec![1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (11, 5),
      (1, 3),
      (0, 3),
      (0, 3),
      (0, 3),
      (0, 3),
      (0, 3),
      (0, 3),
      (2, 3),
      (0, 3),
      (2, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 15, 5, -1).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_5() {
    let dat_arr181 = vec![0, 3, 2, 3, 0, 3, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (8, 5),
      (0, 3),
      (3, 3),
      (2, 3),
      (0, 2),
      (3, 3),
      (0, 3),
      (3, 3),
      (3, 3),
      (2, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 19, 5, 3).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_6() {
    let dat_arr181 = vec![5, 2, 3, 4, 5, 5, 5, 4, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (11, 5),
      (5, 3),
      (2, 3),
      (3, 3),
      (4, 3),
      (5, 3),
      (5, 3),
      (5, 3),
      (4, 3),
      (3, 3),
      (3, 3),
      (3, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 15, 5, -1).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_7() {
    let dat_arr181 = vec![6, 0, 4, 0, 0, 0, 6, 3, 1, 3, 4, 4, 5, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (13, 5),
      (6, 3),
      (0, 3),
      (4, 3),
      (3, 2),
      (6, 3),
      (3, 3),
      (1, 3),
      (3, 3),
      (4, 3),
      (4, 3),
      (5, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 19, 5, 3).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_8() {
    let dat_arr181 = vec![5, 3, 4, 4, 4, 4, 5, 4, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (11, 5),
      (5, 3),
      (3, 3),
      (4, 3),
      (4, 3),
      (4, 3),
      (4, 3),
      (5, 3),
      (4, 3),
      (2, 3),
      (3, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 15, 5, -1).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_9() {
    let dat_arr181 = vec![0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls =
      ExactCallWriter::from(vec![(4, 5), (0, 3), (0, 3), (1, 3), (0, 2), (1, 3)]);
    pure_fn218(&mut expected_calls, &dat_arr181, 19, 5, 3).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_10() {
    let dat_arr181 = vec![4, 7, 5, 0, 0, 0, 8, 8, 1, 3, 5, 5, 4, 3, 6, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (15, 5),
      (4, 3),
      (65534, 4),
      (5, 3),
      (3, 2),
      (65534, 5),
      (65534, 5),
      (1, 3),
      (3, 3),
      (5, 3),
      (5, 3),
      (4, 3),
      (3, 3),
      (6, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 19, 5, 3).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_11() {
    let dat_arr181 = vec![6, 3, 3, 4, 6, 5, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (11, 5),
      (6, 3),
      (3, 3),
      (3, 3),
      (4, 3),
      (6, 3),
      (5, 3),
      (3, 3),
      (3, 3),
      (3, 3),
      (3, 3),
      (3, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 15, 5, -1).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_12() {
    let dat_arr181 = vec![7, 3, 4, 5, 4, 6, 7, 4, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (11, 5),
      (65534, 4),
      (3, 3),
      (4, 3),
      (5, 3),
      (4, 3),
      (6, 3),
      (65534, 4),
      (4, 3),
      (3, 3),
      (2, 3),
      (2, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 15, 5, -1).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_13() {
    let dat_arr181 = vec![5, 3, 3, 5, 4, 3, 3, 3, 4, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (11, 5),
      (5, 3),
      (3, 3),
      (3, 3),
      (5, 3),
      (4, 3),
      (3, 3),
      (3, 3),
      (3, 3),
      (4, 3),
      (3, 3),
      (4, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 15, 5, -1).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_14() {
    let dat_arr181 = vec![5, 5, 3, 4, 4, 3, 3, 4, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (11, 5),
      (5, 3),
      (5, 3),
      (3, 3),
      (4, 3),
      (4, 3),
      (3, 3),
      (3, 3),
      (4, 3),
      (3, 3),
      (3, 3),
      (3, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 15, 5, -1).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_15() {
    let dat_arr181 = vec![4, 5, 5, 0, 0, 0, 7, 7, 2, 2, 3, 5, 6, 3, 4, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (15, 5),
      (4, 3),
      (5, 3),
      (5, 3),
      (3, 2),
      (65534, 4),
      (65534, 4),
      (2, 3),
      (2, 3),
      (3, 3),
      (5, 3),
      (6, 3),
      (3, 3),
      (4, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 19, 5, 3).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_16() {
    let dat_arr181 = vec![0, 6, 5, 0, 0, 0, 6, 6, 1, 2, 5, 6, 3, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (13, 5),
      (0, 3),
      (6, 3),
      (5, 3),
      (3, 2),
      (6, 3),
      (6, 3),
      (1, 3),
      (2, 3),
      (5, 3),
      (6, 3),
      (3, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 19, 5, 3).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_17() {
    let dat_arr181 = vec![1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (9, 5),
      (1, 3),
      (0, 3),
      (0, 3),
      (0, 3),
      (0, 3),
      (0, 3),
      (0, 3),
      (0, 3),
      (1, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 15, 5, -1).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_18() {
    let dat_arr181 = vec![3, 3, 3, 0, 0, 5, 5, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (9, 5),
      (3, 3),
      (3, 3),
      (3, 3),
      (2, 2),
      (5, 3),
      (5, 3),
      (1, 3),
      (4, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 19, 5, 3).unwrap();
    expected_calls.assert_drained();
  }

  #[test]
  fn test_fn218_19() {
    let dat_arr181 = vec![0, 5, 5, 0, 0, 0, 6, 3, 1, 3, 3, 5, 6, 0, 0, 0, 0, 0, 0];
    let expected_calls = ExactCallWriter::from(vec![
      (13, 5),
      (0, 3),
      (5, 3),
      (5, 3),
      (3, 2),
      (6, 3),
      (3, 3),
      (1, 3),
      (3, 3),
      (3, 3),
      (5, 3),
      (6, 3),
    ]);
    pure_fn218(&mut expected_calls, &dat_arr181, 19, 5, 3).unwrap();
    expected_calls.assert_drained();
  }
}
