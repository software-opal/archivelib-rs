#![no_main]

#[cfg(fuzzing)]
#[macro_use]
extern crate libfuzzer_sys;

#[cfg(fuzzing)]
fuzz_target!(|data: &[u8]| {
  let _ = archivelib::do_compress(&data);
});
