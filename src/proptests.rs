use crate::CompressionLevel;
use ::proptest::collection::vec;
use ::proptest::prelude::*;
use ::proptest::test_runner::TestCaseError;
use ::proptest::*;
use std::iter;

fn raw_data_strat() -> impl Strategy<Value = Vec<u8>> {
  vec(0..=0xFFu8, 1..(1 << 16))
}
fn level_strat() -> impl Strategy<Value = CompressionLevel> {
  prop_oneof![
    Just(CompressionLevel::Level0),
    Just(CompressionLevel::Level1),
    Just(CompressionLevel::Level2),
    Just(CompressionLevel::Level3),
    Just(CompressionLevel::Level4),
  ]
}
prop_compose! {
  fn repeating_data_strat()(repeat in 0..1024usize, vector in vec(0..=0xFFu8, 1..256)) -> Vec<u8> {

    iter::repeat(vector).take(repeat).flatten().collect()}
}

fn proptest_compress(vec: Vec<u8>, level: u8) -> Result<(), TestCaseError> {
  use crate::do_compress_level;
  use archivelib_sys::do_compress_level as do_compress_level_sys;

  let real_data = match do_compress_level_sys(&vec, level) {
    Ok(data) => data,
    Err(err) => {
      return Err(TestCaseError::reject(format!(
        "Compression failed: {}",
        err
      )));
    }
  };
  let test_data = match do_compress_level(&vec, level) {
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

fn proptest_expand(vec: Vec<u8>, level: CompressionLevel) -> Result<(), TestCaseError> {
  use crate::do_decompress_level;
  use archivelib_sys::do_compress_level;

  let compressed_data = match do_compress_level(&vec, level.compression_level()) {
    Ok(data) => data,
    Err(err) => {
      return Err(TestCaseError::reject(format!(
        "Compression failed: {}",
        err
      )));
    }
  };
  let test_data = match do_decompress_level(&compressed_data, level.compression_level()) {
    Ok(data) => data,
    Err(err) => {
      return Err(TestCaseError::fail(format!(
        "Decompression failed: {:?}",
        err
      )));
    }
  };
  prop_assert_eq!(
    vec,
    test_data.to_vec(),
    "Decompression produced different results"
  );
  Ok(())
}

fn proptest_new_expand(vec: Vec<u8>, level: CompressionLevel) -> Result<(), TestCaseError> {
  use crate::expand_new::do_expand_level;
  use archivelib_sys::do_compress_level;

  let compressed_data = match do_compress_level(&vec, level.compression_level()) {
    Ok(data) => data,
    Err(err) => {
      return Err(TestCaseError::reject(format!(
        "Compression failed: {}",
        err
      )));
    }
  };
  let test_data = match do_expand_level(&compressed_data, level) {
    Ok(data) => data,
    Err(err) => {
      return Err(TestCaseError::fail(format!(
        "Decompression failed: {:?}",
        err
      )));
    }
  };
  prop_assert_eq!(
    vec,
    test_data.to_vec(),
    "Decompression produced different results. Compressed vec: {:?}",
    compressed_data,
  );
  Ok(())
}

proptest! {
  // #[test]
  // fn test_compression_port(vec in raw_data_strat(), level in level_strat()) {
  //   proptest_compress(vec, level)?
  // }
  // #[test]
  // fn test_compression_port_with_repeat_strat(vec in repeating_data_strat(), level in level_strat())  {
  //   proptest_compress(vec, level)?
  // }
  #[test]
  fn test_new_expand_port(vec in repeating_data_strat(), level in level_strat()) {
    proptest_new_expand(vec, level)?
  }
  #[test]
  fn test_expand_port(vec in repeating_data_strat(), level in level_strat()) {
    proptest_expand(vec, level)?
  }
  #[test]
  fn test_new_expand_port_with_raw_strat(vec in raw_data_strat(), level in level_strat()) {
    proptest_new_expand(vec, level)?
  }
  #[test]
  fn test_expand_port_with_raw_strat(vec in raw_data_strat(), level in level_strat()) {
    proptest_expand(vec, level)?
  }
}
