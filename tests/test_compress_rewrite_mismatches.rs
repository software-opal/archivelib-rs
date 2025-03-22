use archivelib::assert_bytes_eq;

#[macro_use]
mod utils;

#[test]
fn test_mismatch_long_with_trailing_zero() {
  let input = hex!("
    00 00 00 a5 1d ff 82 72  01 04 00 00 00 00 00 c7  05 0b dd 56 3d 48 1b 42  00 34 11 f7 04 00 00 00
    82 00 00 00 00
  ");
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let output = hex!("
    00 1E 44 4D A2 9F FF 9D  C4 48 93 39 80 49 50 A2  54 C0 75 9F C0 E6 06 B0  05 80 A9 0F E7 40 C8 00
    88 19 97 53 0C 6F B2 1A  73 F7 7A 5A 35 27 0E 28  91 4D F8
  ");
  assert_bytes_eq!(output, &result.unwrap()[..])
}

#[test]
#[cfg(feature = "sys")]
fn test_mismatch_data() {
  // This exposed a bug in the run finding code. The code performed bounds checking whilst looking
  //  for runs, which didn't match the original implementation which read beyond the array's
  //  boundaries if the run continued beyond the boundaries.
  let input = hex!("00 00 00 00 39 00 00 00");
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let sys_result = archivelib::sys::do_compress_level(&input, 0);
  let output = hex!("00 05 28 05 3F F8 49 2C  A7 4C 84 02 46 98");
  assert_bytes_eq!(&output, &result.unwrap()[..]);
  assert_bytes_eq!(&output, &sys_result.unwrap()[..]);
}

#[test]
#[cfg(feature = "sys")]
fn test_mismatch_data_replacing_nul_with_another_byte() {
  let input = hex!("01 01 01 01 39 01 01 01");
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let sys_result = archivelib::sys::do_compress_level(&input, 0);
  let output = hex!("00 05 2A 09 3F F9 88 DA  C9 BA 43 20 91 BC");
  assert_bytes_eq!(&output, &sys_result.unwrap()[..]);
  assert_bytes_eq!(&output, &result.unwrap()[..]);
}

#[test]
#[cfg(feature = "sys")]
fn test_short_data_with_trailing_null_bytes() {
  // This exposed a bug in the fix for the run finding code. It correctly performed bounds checking
  //  after finding the longest run, but this could cause the run to now be too short. The code
  //  incorrectly returned this too-short run as a valid run.
  let input = hex!("40 00 00 00");
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let sys_result = archivelib::sys::do_compress_level(&input, 0);
  let output = hex!("00 05 2A 09 3F F9 88 DA  C9 BA 43 20 91 BC");
  assert_bytes_eq!(&output, &sys_result.unwrap()[..]);
  assert_bytes_eq!(&output, &result.unwrap()[..]);
}
