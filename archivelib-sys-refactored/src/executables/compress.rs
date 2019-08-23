mod util;

run! {
  (input, level) => {
    archivelib_sys::do_compress_level(&input, level.compression_level())
  }
}
