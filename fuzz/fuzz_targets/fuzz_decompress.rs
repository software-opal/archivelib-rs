#![no_main]

#[cfg(fuzzing)]
#[macro_use]
extern crate libfuzzer_sys;

#[cfg(fuzzing)]
#[macro_use]
extern crate archivelib;

#[cfg(fuzzing)]
fuzz_target!(|data: &[u8]| { check_rust_against_sys_decompress!(data); });
