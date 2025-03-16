mod util;

run! {
  (input, level) => {
    archivelib::do_compress_level(input, level)
  }
}
