#![no_main]

#[cfg(fuzzing)]
#[macro_use]
extern crate libfuzzer_sys;

#[cfg(fuzzing)]
#[macro_use]
extern crate archivelib;

#[cfg(fuzzing)]
fuzz_target!(|data: &[u8]| {
  let compressed = archivelib::do_compress(&data).unwrap();
  let decompressed = archivelib::do_decompress(&compressed).unwrap();
  assert_eq!(data, &decompressed[..]);
});
