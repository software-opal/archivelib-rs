#[macro_use]
extern crate afl;

fn main() {
  fuzz!(|data: &[u8]| {
    let compressed = archivelib_sys::do_compress(&data).unwrap();
    let decompressed = archivelib::do_decompress(&compressed).unwrap();
    assert_eq!(decompressed[..], data[..]);
  });
}
