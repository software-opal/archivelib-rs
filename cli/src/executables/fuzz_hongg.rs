#![cfg(feature = "fuzz-hfuzz")]

#[macro_use]
extern crate honggfuzz;
#[macro_use]
extern crate archivelib;

use std::error::Error;

fn main() {
  loop {
    honggfuzz::fuzz!(|input| {
      check_rust_against_sys_decompress!(input);
    })
  }
}
