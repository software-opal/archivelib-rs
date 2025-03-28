//! The C library handles end of file in a somewhat confusing way. The library will repeatedly
//! read the last 8 bits of a file until the parent loop has read past the end of file enough times

#[macro_use]
mod utils;

check_decompress_matches! {
  // SHA1: da39a3ee5e6b4b0d3255bfef95601890afd80709
  empty_file([], [0x00]);
  // SHA1: 8dc00598417d4eb788a77ac6ccef3cb484905d8b
  #[ignore = "New implementation has different bugs"]
  short_file_05([0x05], [0x00]);
  // SHA1: 6c87e8951299d8a532146a93911048146b6fe1e0
  #[ignore = "New implementation has different bugs"]
  short_file_00_03([0x00, 0x03], [0x00]);
  // SHA1: ccecfa087bf90801d87177d05bfe3ef4cafaf2da
  semivalid([0x00, 0x01, 0x00, 0x00, 0x1F, 0xE0, 0x00], []);
  // SHA1: 696a34af1fc31e073ce7a3b6bb6bae2bfb47917d
  compressed_hus_data(
    [
      0x00, 0x18, 0x40, 0x68, 0x61, 0xB5, 0xFF, 0x0D, 0x9F, 0x43, 0xD1, 0x10, 0xBC, 0xA0, 0xCB, 0x89,
      0xDA, 0x80, 0x16, 0x77, 0x00, 0x01, 0xB6, 0x7B, 0x39, 0xF0,
    ],
    {
      let mut v = vec![ 0x88];
      v.append(&mut vec![0x81; 7]);
      v.append(&mut vec![0x80; 3407]);
      v.append(&mut vec![0x88; 1]);
      v.append(&mut vec![0x81; 8]);
      v.append(&mut vec![0x80; 1]);
      v.append(&mut vec![0x90; 1]);
      v
    }
  );
  #[ignore = "New implementation has different bugs"]
  short_file_05_05([0x05, 0x05], [0x00]);
}
