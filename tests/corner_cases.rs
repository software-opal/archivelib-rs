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
}

#[test]
fn slice_index_starts_after_ends() {
  let input = [0x27, 0x27, 0x30, 0x48];

  match archivelib::do_decompress(&input) {
    Err(e) => assert_eq!("Binary tree error: Type1", e),
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
