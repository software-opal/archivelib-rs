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
    for i in (0 as usize)..(1 << 10) {
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
fn test_compressing_hakuna_matata() {
  let input = "
Hakuna matata
What a wonderful phrase
Hakuna matata
Ain't no passing craze
It means no worries for the rest of your days
It's our problem-free philosophy
Hakuna matata

Why, when he was a young warthog
When I was a young warthog
Very nice, thanks

He found his aroma lacked a certain appeal
He could clear the Savannah after every meal
I'm a sensitive soul, though I seem thick-skinned
And it hurt that my friends never stood downwind

And oh, the shame (he was ashamed)
Thought of changin' my name (what's in a name?)
But I got downhearted (how did ya feel?)
Every time that I
Hey, Pumbaa! Not in front of the kids
Oh, sorry

Hakuna matata
What a wonderful phrase
Hakuna matata
Ain't no passing craze
It means no worries for the rest of your days
Yeah, sing it kid
It's our problem-free philosophy
Hakuna matata

Hakuna matata, hakuna matata
Hakuna matata
It means no worries for the rest of your days
It's our problem-free philosophy
Hakuna matata (hakuna matata, hakuna matata)
Hakuna matata (hakuna matata, hakuna matata)
Hakuna matata
Hakuna matata

Hakuna matata (hakuna matata, hakuna matata)
(Hakuna matata, hakuna matata)
Hakuna matata (hakuna matata, hakuna matata)
(Hakuna matata, hakuna matata)
(Hakuna matata, hakuna matata)
".as_bytes();
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("00 08 30 69 67 FF 11 98  C2 44 79 D0 22 05 39 70  A3 3C");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}
#[test]
fn test_compressing_a_little_bit_of_data() {
  let input = "abcdabcdZabcd".as_bytes();
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("00 08 30 69 67 FF 11 98  C2 44 79 D0 22 05 39 70  A3 3C");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_compressing_a_run_of_identical_data() {
  let input = "aaaa".as_bytes();
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("00 03 28 04 4B FE 26 E4  54 74 E0 04 C0");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_compressing_a_small_data_block() {
  let input = "a".as_bytes();
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("00 03 28 04 4B FE 26 E4  54 74 E0 04 C0");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_compressing_known_data_ab() {
  let input = "GGaXaGXYGYXaZXGYbXbYcZZZ".as_bytes();
  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
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
