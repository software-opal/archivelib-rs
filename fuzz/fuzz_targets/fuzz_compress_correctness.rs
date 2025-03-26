#![cfg_attr(fuzzing, no_main)]

#[macro_use]
mod helper;

fuzz_with_main! { |data: &[u8]| {
  let compressed_sample = archivelib_sys::do_compress(&data).unwrap();
  let compressed_test = archivelib::do_compress(&data).unwrap();
  assert_eq!(compressed_sample[..], compressed_test[..]);
}}
