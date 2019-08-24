mod util;

run! {
  (input, level) => {
    archivelib_sys::do_decompress_level(&input, level.compression_level())
  }
}
