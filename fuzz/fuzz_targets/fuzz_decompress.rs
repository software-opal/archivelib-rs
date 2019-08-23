#![no_main]
#[macro_use]
extern crate libfuzzer_sys;
#[macro_use]
extern crate archivelib;

fuzz_target!(|data: &[u8]| { check_rust_against_sys_decompress!(data) });
