use archivelib_sys::do_compress_level;
use crate::do_decompress_level;
use proptest::collection::vec;
use proptest::prelude::*;
use proptest::test_runner::TestCaseError;

fn raw_data_strat() -> impl Strategy<Value = Vec<u8>> {
  vec(0..=0xFFu8, 1..(1 << 16))
}
fn level_strat() -> impl Strategy<Value = u8> {
  0..=4u8
}

proptest! {
  #[test]
  fn test_compress_algorithm(vec in raw_data_strat(), level in level_strat()) {
    let data =  match do_compress_level(&vec, level) {
      Ok(data) => data,
      Err(err) => return Err(TestCaseError::fail(err)),
    };
  }
}
proptest! {
  #[test]
  fn test_decompression_port(vec in raw_data_strat(), level in level_strat()) {
    let data = match do_compress_level(&vec, level) {
      Ok(data) => data,
      Err(err) => return Err(TestCaseError::reject(format!("Compression failed: {}", err))),
    };
    let result = match do_decompress_level(&data, level) {
      Ok(data) => data,
      Err(err) => return Err(TestCaseError::fail(err)),
    };
    prop_assert_eq!(&vec[..], &result[..], "Data is not identical after decompression");
  }
}
