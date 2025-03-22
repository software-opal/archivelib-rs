#![no_main]

#[cfg(fuzzing)]
#[macro_use]
extern crate libfuzzer_sys;

#[cfg(fuzzing)]
fuzz_target!(|data: &[u8]| {
  let compressed = archivelib_sys::do_compress(&data).unwrap();
  let decompressed = archivelib::do_decompress(&compressed).unwrap();
  assert_eq!(decompressed[..], data[..]);
});
