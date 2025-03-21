use std::io::Read;

use archivelib::assert_bytes_eq;
use flate2::{Compress, Compression};

#[macro_use]
mod utils;

#[test]
fn test_compressing_lots_of_data_with_breaks() {
  let input = {
    let mut input: Vec<u8> = Vec::with_capacity(1 << 12);
    input.extend("aByZ".as_bytes());
    for i in 0_usize..(1 << 10) {
      input.extend_from_within(0..3);
      input.push((i & 255) as u8);
    }
    input
  };
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  assert!(result.is_ok())
}

#[test]
fn test_compressing_lots_of_data() {
  let input = {
    let mut input: Vec<u8> = Vec::with_capacity(1 << 12);
    input.extend("aByZ".as_bytes());
    for _ in 0..(1 << 10) {
      input.extend_from_within(0..3);
    }
    input
  };
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let compressed_ab =
    hex!("00 12 3B A8 A2 1F FC 2E  40 37 8F A0 1A 39 27 2B  6F 86 41 95 97 BA 49 24  92 4B AF 00");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_compressing_a_little_bit_of_data() {
  let input = "abcdabcdZabcd".as_bytes();
  let result = archivelib::do_compress_level(input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("00 08 30 69 67 FF 11 98  C2 44 79 D0 22 05 39 70  A3 3C");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_compressing_nothing() {
  let input = "".as_bytes();
  let result = archivelib::do_compress_level(input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("00 01 00 00 1F E0 00");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_compressing_1_a() {
  let input = "a".as_bytes();
  let result = archivelib::do_compress_level(input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("00 02 20 04 3F F1 36 C4  40 04");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_compressing_a_run_of_identical_data() {
  let input = "aaaa".as_bytes();
  let result = archivelib::do_compress_level(input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("00 03 28 04 4B FE 26 E4  54 74 E0 04 C0");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_compressing_the_example_data() {
  let input = "I am what I am; ABABABAB".as_bytes();
  let result = archivelib::do_compress_level(input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!(
    "
    00 11 43 49 B5 4F FA 0C  F2 06 E0 A8 39 01 FC 38
    18 3B 69 3A DA 5C DC 54  40 50 2A 32 55 9B 9F 0C
    FC FC
  "
  );
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}
#[test]
fn test_compressing_a_small_data_block() {
  let input = "aabc".as_bytes();
  let result = archivelib::do_compress_level(input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("00 05 30 04 6D 7F C4 DD  76 1A 00 0D 70");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_compressing_known_data_ab() {
  let input = "ab".as_bytes();
  let result = archivelib::do_compress_level(input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("00 03 28 04 4B FE 26  F3  0F 80 13");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}

#[ignore]
#[test]
fn test_compressing_using_zlib() {
  let input = "ab".as_bytes();
  let archivelib_result = hex!("00 03 28 04 4B FE 26  F3  0F 80 13");

  let zlib_conf = Compress::new_with_window_bits(Compression::new(0), false, 10);
  let mut writer = flate2::bufread::ZlibEncoder::new_with_compress(input, zlib_conf);

  let mut output = vec![];
  writer.read_to_end(&mut output).unwrap();

  assert_bytes_eq!(archivelib_result, output);
}

#[test]
fn test_decompressing_known_data_ab() {
  let compressed_ab = hex!("00 03 28 04 4b fe 26  F3  0f 80 13");
  let result = archivelib::do_decompress(&compressed_ab);
  assert_eq!("ab".as_bytes(), &result.unwrap()[..])
}
#[test]
fn test_decompressing_known_data_ba() {
  let compressed_ba = hex!("00 03 28 04 4b fe 26  DB  0f 80 13");
  let result = archivelib::do_decompress(&compressed_ba);
  assert_eq!("ba".as_bytes(), &result.unwrap()[..])
}
