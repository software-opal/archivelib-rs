#![no_main]
#[macro_use]
extern crate libfuzzer_sys;

fuzz_target!(|data: &[u8]| {
  let decompressed = archivelib_rs_tdd::do_decompress(&data);
});
