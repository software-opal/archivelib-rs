#![no_main]
#[macro_use]
extern crate libfuzzer_sys;

fuzz_target!(|data: &[u8]| {
  let _ = archivelib::do_decompress(&data);
});
