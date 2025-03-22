use archivelib::assert_bytes_eq;

#[macro_use]
mod utils;


#[test]
fn test_mismatch_data() {
  let input = hex!("
    00 00 00 a5 1d ff 82 72  01 04 00 00 00 00 00 c7  05 0b dd 56 3d 48 1b 42  00 34 11 f7 04 00 00 00
    82 00 00 00 00
  ");
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);;
  let output = hex!("
    00 1E 44 4D A2 9F FF 9D  C4 48 93 39 80 49 50 A2  54 C0 75 9F C0 E6 06 B0  05 80 A9 0F E7 40 C8 00
    88 19 97 53 0C 6F B2 1A  73 F7 7A 5A 35 27 0E 28  91 4D F8
  ");
  assert_bytes_eq!( output, &result.unwrap()[..])
}
