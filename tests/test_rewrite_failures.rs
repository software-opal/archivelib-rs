use archivelib::assert_bytes_eq;

#[test]
fn test_rewrite_compress_decompress_failure() {
  // The rewrite should be able to decompress all valid documents.
  let input = include_bytes!("data/rewrite/invalid_offset_error.in");

  let compressed = archivelib::do_compress(input).unwrap();

  let uncompressed = archivelib::expand_rewrite::do_decompress(&compressed).unwrap();

  assert_bytes_eq!(input, uncompressed);
}
