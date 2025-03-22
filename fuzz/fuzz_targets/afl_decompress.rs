#[macro_use]
extern crate archivelib;

#[macro_use]
extern crate afl;

fn main() {
  fuzz!(|data: &[u8]| {
    check_rust_against_sys_decompress!(data);
  });
}
