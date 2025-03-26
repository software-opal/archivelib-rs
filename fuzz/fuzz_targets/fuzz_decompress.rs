#![cfg_attr(fuzzing, no_main)]

#[macro_use]
mod helper;

#[macro_use]
extern crate archivelib;

fuzz_with_main! { |data: &[u8]| {
  #![allow(unused_must_use)]
  check_rust_against_sys_decompress!(data);
}}
