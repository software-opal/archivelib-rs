mod util;

run! {
  (input, level) => {
    archivelib::do_decompress_level(input, level)
  }
}
