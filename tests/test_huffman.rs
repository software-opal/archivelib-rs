use archivelib::assert_bytes_eq;

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
fn test_compressing_lots_of_different_data() {
  let input = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ., ".as_bytes();
  let result = archivelib::do_compress_level(input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("
    00 42 54 73 00 AF FF 83  2D 17 38 00 C8 00 00 00  63 00 00 00 5F 6F 80 09  66 9E 8A 6A AE CB 6E BF
    0C 72 CF 4D 76 DF 8E 7A  EF CF 7F 02 10 62 0A 30  E4 12 51 66 1A 71 E8 22  92 6A 2A B2 EC 32 D3 6E
    3A F3 F0 43 14 72 01 F7  E7 F0
  ");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}

#[test]
fn test_compress_byte_encoding_bit_length_tree_depth_of_7() {
  let mut input = Vec::with_capacity(256);
  input.extend(32_u8..=255);
  // input.extend((0_u8..=255).rev());
  input.extend(
    concat!(
      "  a0a1a0b0b1b0c0c1c01d01d1d0e01e1e10f0ff0g0h0i0j0k0l0m0n0o0p0q0r0s0t0u0v0w0x0y0z0123456\n",
      "  789a0Aa9b0Bb9c0Cc9d0Dd9e0Ee9f0Ff9g0Gg9h0Hh9i0Ii9j0Jj9k0Kk9l0Ll9m0Mm9n0Nn9o0Oo9p0Pp9q0\n",
      "  Qq9r0Rr9s0Ss9t0Tt9u0Uu9v0Vv9w0Ww9x0Xx9y0Yy9z0Zz9., 000",
    )
    .as_bytes(),
  );

  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let compressed_ab = hex!("
    01 B9 60 D6 EE 11 A3 FF  F9 DE 00 F0 00 A9 FB 80  7F 54 2B 5A AA AA AA AA  B5 54 3B BB 77 6D B6 DD
    B6 ED B6 DD DB 00 00 01  55 55 55 00 00 00 00 00  00 00 00 00 00 00 00 1D  F0 3C E5 94 DA 60 06 11
    96 DB 75 BE E1 71 B9 5C  EE 97 5B B5 DE 22 26 2A  F0 0D BC DE AF 77 CB ED  FA FE 58 08 B8 C8 DC 0E
    0B 07 1D 1E C2 41 8C 8C  94 9C A4 AC B4 BC C4 CC  D4 DC E4 EC F4 FD 05 0D  13 2A 3A 4A 5A 6A 7C 26
    17 0C 71 E8 33 45 26 8D  5B 37 70 E5 37 4E DE 28  F5 F3 F8 10 55 5A 14 3C  3E 23 13 8A C5 E3 31 B8
    EC 7E 43 23 92 C9 E5 32  B9 6C BE 63 33 9A CD E7  2A 2A 6A AA EB 2B 6B AB  EC 2C 6C AC ED 33 B9 EC
    FE 83 43 A2 D1 E9 34 BA  6D 3E A3 53 AA D5 EB 35  BA ED 7E C3 63 B2 D9 ED  36 BB 6D BE E3 73 BA DD
    EF 37 BB ED FF 03 83 C2  E1 F1 38 BC 6E 3F 23 93  CA E5 F3 39 BC EE 7F 43  A3 D2 E9 F5 3A BD 6E BF
    63 B3 DA ED F7 3B BD EE  FF 83 C3 E2 F1 F9 3C BE  6F 3F A3 D3 EA F5 FB 3D  BE EF 7F C3 E3 F2 F9 FD
    3E BF 6F BF E3 F3 FA FD  FF 3F BF E3 0C 38 1C 69  C0 F0 79 A7 84 02 06 A0  0D 66 B8 5F 08 83 51 5E
    84 82 49 06 81 A8 6C 1B  87 01 C8 4C 3A 0E C3 C0  A0 7A 1F 07 E2 00 82 15  0B 08 42 1F FD 1F B6 18
    62 E9 11 C2 38 E2 3C 47  9E 48 06 08 13 31 20 CC  91 0C 51 24 84 8A 44 D0  49 34 26 A2 4D A9 36 12
    8D 89 B8 95 6E 4E 04 B3  82 72 25 DC 92 62 61 32  74 26 5D 13 B1 34 EC 9E  09 B7 84 A0 5F AF 38 A1
    3D 13 AF 49 F0 9E 7C 4F  C4 FB F2 80 28 20 14 11  43 04 95 14 4A 92 C1 92  C5 08 51 C2 28 62 92 19
    45 44 18 00 B5 80
  ");
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
