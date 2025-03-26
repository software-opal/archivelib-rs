#![cfg_attr(fuzzing, no_main)]

#[macro_use]
mod helper;

fuzz_with_main! { |data: &[u8]| {
  let _ = archivelib::do_compress(&data);
}}
