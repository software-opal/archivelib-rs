#![no_main]
#[macro_use]
extern crate libfuzzer_sys;

fuzz_target!(|data: &[u8]| {
  let compressed_sample = archivelib_sys::do_compress(&data).unwrap();
  let compressed_test = archivelib_rs_tdd::do_compress(&data).unwrap();
  assert_eq!(compressed_sample[..], compressed_test[..]);
});
