#![cfg_attr(fuzzing, no_main)]

#[macro_use]
mod helper;

fuzz_with_main! { |data: &[u8]| {
  let compressed = archivelib_sys::do_compress(&data).unwrap();
  let decompressed = archivelib::do_decompress(&compressed).unwrap();
  assert_eq!(decompressed[..], data[..]);
}}
