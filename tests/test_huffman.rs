use std::io::Read;

use archivelib::assert_bytes_eq;
use flate2::{Compress, Compression};

#[macro_use]
mod utils;

#[test]
fn test_compressing_lots_of_data() {
  let input = {
    let mut input: Vec<u8> = Vec::with_capacity(1 << 12);
    input.extend("aByZ".as_bytes());
    for _ in 0..(1<<10) {input.extend_from_within(0..3);}
    input
  };
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("00 03 28 04 4b fe 26  f3  0f 80 13");
  assert_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_compressing_known_data_ab() {
  let input = "ab".as_bytes();
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("00 03 28 04 4b fe 26  f3  0f 80 13");
  assert_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_compressing_using_zlib() {
  let input = "ab".as_bytes();
  let archivelib_result = hex!("00 03 28 04 4b fe 26  f3  0f 80 13");

  let zlib_conf = Compress::new_with_window_bits(Compression::new(0), false, 10);
  let mut writer = flate2::bufread::ZlibEncoder::new_with_compress(input, zlib_conf);

  let mut output = vec![];
  writer.read_to_end(&mut output).unwrap();

  assert_bytes_eq!(archivelib_result, output);
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
