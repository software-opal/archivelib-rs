#![cfg(feature = "fuzz-afl")]

#[macro_use]
extern crate afl;
#[macro_use]
extern crate archivelib;

use std::error::Error;

fn main() {
  afl::fuzz!(|input| {
    check_rust_against_sys_decompress!(input);
  });
}
