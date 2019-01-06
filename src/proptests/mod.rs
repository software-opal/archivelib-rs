use proptest::collection::vec;
use proptest::prelude::*;
use proptest::test_runner::TestCaseError;

fn raw_data_strat() -> impl Strategy<Value = Vec<u8>> {
  vec(0..=0xFFu8, 1..(1 << 16))
}
fn level_strat() -> impl Strategy<Value = u8> {
  0..=4u8
}
fn repeating_data_strat() -> BoxedStrategy<Vec<u8>> {
  (0..2048usize, vec(0..=0xFFu8, 1..2048))
    .prop_map(|(repeat, vector)| {
      vector
        .iter()
        .cycle()
        .take(vector.len() * repeat)
        .map(|&v| v)
        .collect()
    })
    .boxed()
}

fn proptest_compress(vec: Vec<u8>, level: u8) -> Result<(), TestCaseError> {
  let real_data = match archivelib_sys::do_compress_level(&vec, level) {
    Ok(data) => data,
    Err(err) => {
      return Err(TestCaseError::reject(format!(
        "Compression failed: {}",
        err
      )));
    }
  };
  let test_data = match crate::do_compress_level(&vec, level) {
    Ok(data) => data,
    Err(err) => return Err(TestCaseError::fail(err)),
  };
  prop_assert_eq!(
    real_data,
    test_data,
    "Compression produced different results"
  );
  Ok(())
}

proptest! {
  #[test]
  fn test_compression_port(vec in raw_data_strat(), level in level_strat()) {
    proptest_compress(vec, level)?
  }
  #[test]
  fn test_compression_port_with_repeat_strat(vec in repeating_data_strat(), level in level_strat())  {
    proptest_compress(vec, level)?
  }
  #[test]
  fn test_decompression_port(vec in raw_data_strat(), level in level_strat()) {
    let data = match archivelib_sys::do_compress_level(&vec, level) {
      Ok(data) => data,
      Err(err) => return Err(TestCaseError::reject(format!("Compression failed: {}", err))),
    };
    let result = match crate::do_decompress_level(&data, level) {
      Ok(data) => data,
      Err(err) => return Err(TestCaseError::fail(err)),
    };
    prop_assert_eq!(&vec[..], &result[..], "Data is not identical after decompression");
  }
    #[test]
    fn test_decompression_port_with_repeat_strat(vec in repeating_data_strat(), level in level_strat()) {
      let data = match archivelib_sys::do_compress_level(&vec, level) {
        Ok(data) => data,
        Err(err) => return Err(TestCaseError::reject(format!("Compression failed: {}", err))),
      };
      let result = match crate::do_decompress_level(&data, level) {
        Ok(data) => data,
        Err(err) => return Err(TestCaseError::fail(err)),
      };
      prop_assert_eq!(&vec[..], &result[..], "Data is not identical after decompression");
    }
}
