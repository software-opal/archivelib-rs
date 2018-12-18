#![no_main]
#[macro_use]
extern crate libfuzzer_sys;

fuzz_target!(|data: &[u8]| {
  let compressed = archivelib_sys::do_compress(&data).unwrap();
  let decompressed = archivelib_rs_tdd::do_decompress(&compressed).unwrap();
  assert_eq!(decompressed[..], data[..]);
});
