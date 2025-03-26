#[macro_use]
mod utils;

check_decompress_matches! {
  run_offset_run_length_gt_15(
    [
      0x30, 0x30, 0x7C, 0xEB, 0xFB, 0xC5, 0xDB, 0x1E, 0xFF, 0x88, 0x00, 0x00, 0x21, 0x24, 0x9D, 0x9D,
      0xEE, 0xEF, 0xD9, 0xF5, 0xB7, 0x32, 0xE6, 0xF0, 0x2D, 0x78, 0x11, 0xB9, 0x7F, 0x25, 0xE0, 0xD7,
      0x6E, 0x5C, 0xCD, 0xCC, 0xF7, 0xB7, 0xDD, 0xEF, 0x64, 0x81, 0x00, 0x02, 0x04, 0x1F, 0xDF, 0xE9,
      0xEF, 0x66, 0xDF, 0xC9, 0x78, 0xD9, 0xA0, 0xE7,
    ],
    [
      0xE3, 0xE3, 0xE3, 0xE3, 0x07, 0x07, 0x07, 0x07,
    ]
  );
  single_byte_wrong(
    *include_bytes!("data/corner_cases/single_byte_wrong.in"),
    *include_bytes!("data/corner_cases/single_byte_wrong.out")
  );
  incorrect_btree1_error_1(
    // cbf13d89be6fb3c9621e47f22f74ce69cdf73c63
    *include_bytes!("data/corner_cases/pytest_failure_1.in"),
    *include_bytes!("data/corner_cases/pytest_failure_1.out")
  );
  incorrect_btree1_error_2(
    // e3d4875cdc9236dee2621e924a205b9dd3e8469d
    *include_bytes!("data/corner_cases/pytest_failure_2.in"),
    *include_bytes!("data/corner_cases/pytest_failure_2.out")
  );
  incorrect_btree1_error_3(
    // 25f76ced735657189060713f36b314f35a033118
    *include_bytes!("data/corner_cases/pytest_failure_3.in"),
    *include_bytes!("data/corner_cases/pytest_failure_3.out")
  );
  run_offset_assertion_failure(
    [
      0x30, 0x30, 0x7C, 0xEB, 0xFB, 0xC5, 0xDB, 0x1E, 0xFF, 0x88, 0x00, 0x00, 0x21, 0x24, 0x9D,
      0x9D, 0xEE, 0xEF, 0xD9, 0xF5, 0xB7, 0x32, 0xE6, 0xF0, 0x2D, 0x78, 0x11, 0xB9, 0x7F, 0x25,
      0xE0, 0xD7, 0x6E, 0x5C, 0xCD, 0xCC, 0xF7, 0xB7, 0xDD, 0xEF, 0x64, 0x81, 0x00, 0x02, 0x04,
      0x1F, 0xDF, 0xE9, 0xEF, 0x66, 0xDF, 0xC9, 0x78, 0xD9, 0xAC, 0x2C, 0xDD, 0x1A, 0x43, 0x30,
    ],
    [
      0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
    ]
  );
}

#[test]
fn slice_index_starts_after_ends() {
  let input = [0x27, 0x27, 0x30, 0x48];

  match archivelib::do_decompress(&input) {
    Err(e) => assert!(
      "Binary tree error: Type1" == e || "Invalid binary tree" == e,
      "Expected binary tree error, got: {:?}",
      e
    ),
    Ok(v) => panic!("Should have failed with BTE1; instead got {:?}", v),
  }
}

#[test]
fn short_file_a2_errors_when_trying_to_write_out_of_array_bounds() {
  // SHA1: 10687feb9716c9502d9a40fdfe3bb339055c8651
  // This test case doesn't error in the system library the same way because the system library
  // happily writes out of bounds without aborting.
  let input = [0xA2];
  match archivelib::do_decompress(&input) {
    Err(e) => assert_eq!("Invariant Failure", e),
    Ok(v) => panic!("Should have failed with BTE1; instead got {:?}", v),
  }
}

#[test]
fn attempt_to_add_with_overflow_in_expand() {
  // SHA1: 4623dfacad1a3cfddcb67b1c9747c10a2c6eb5fd
  // The system library detects that the requested output size is over it's limit.
  let input = [0x00, 0x00, 0x00, 0x00, 0x10, 0xe0, 0x00];
  match archivelib::do_decompress(&input) {
    Err(e) => assert_eq!("IOError: failed to write whole buffer", e),
    Ok(v) => panic!("Should have failed with IOError; instead got {:?}", v),
  }
}
