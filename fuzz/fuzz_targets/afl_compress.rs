#[macro_use]
extern crate afl;

fn main() {
  fuzz!(|data: &[u8]| {
    let _ = archivelib::do_compress(&data);
  });
}
