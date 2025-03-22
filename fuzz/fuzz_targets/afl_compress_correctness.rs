#[macro_use]
extern crate afl;

fn main() {
  fuzz!(|data: &[u8]| {
    let compressed_sample = archivelib_sys::do_compress(&data).unwrap();
    let compressed_test = archivelib::do_compress(&data).unwrap();
    assert_eq!(compressed_sample[..], compressed_test[..]);
  });
}
