#[macro_use]
mod utils;

#[test]
fn test_compressing_known_data_ab() {
  let input = "ab".as_bytes();
  let result = archivelib::do_compress(input);
  let compressed_ab = hex!("00 03 28 04 4b fe 26  f3  0f 80 13");
  assert_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_decompressing_known_data_ab() {
  let compressed_ab = hex!("00 03 28 04 4b fe 26  f3  0f 80 13");
  let result = archivelib::do_decompress(&compressed_ab);
  assert_eq!("ab".as_bytes(), &result.unwrap()[..])
}
#[test]
fn test_decompressing_known_data_ba() {
  let compressed_ba = hex!("00 03 28 04 4b fe 26  DB  0f 80 13");
  let result = archivelib::do_decompress(&compressed_ba);
  assert_eq!("ba".as_bytes(), &result.unwrap()[..])
}
