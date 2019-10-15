/// The tests in this file cause the system library to crash or behave erratically.

#[test]
fn test_library_prevents_decompress_zipbomb() {
  // SHA1: f0c957104bb1b80c9d125d9c8cbb3f06fbf2ab1a
  // Found by fuzzing, expands to [0;65537]
  let input = [0x00, 0x00, 0x00, 0x04];
  let result = archivelib::do_decompress(&input);
  assert_eq!(
    result,
    Err("IOError: failed to write whole buffer".to_owned())
  )
}

#[test]
fn test_library_prevents_memory_out_of_bounds_access() {
  // SHA1: adad2ca7ab313add6e955f704719e03d5229e4d0
  // Found by fuzzing, causes `malloc(): memory corruption`
  let input = [0xE3];
  let result = archivelib::do_decompress(&input);
  assert_eq!(result, Err("Invariant Failure".to_owned()))
}
#[test]
fn test_library_prevents_free_with_invalid_pointer() {
  // SHA1: da4b9237bacccdf19c0760cab7aec4a8359010b0
  // Found by fuzzing, causes `free(): invalid pointer`
  let input = [0x32];
  let result = archivelib::do_decompress(&input);
  assert_eq!(result, Err("Invariant Failure".to_owned()))
}

#[test]
fn test_star_vip_incorrect_offset_causing_crash() {
  let input = [
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11,
    0x43, 0x68, 0x82, 0x27, 0xFF, 0x0D, 0x9B, 0xE4, 0x6C, 0x61, 0xB4, 0x81, 0x86, 0xBA,
  ];
  let result = archivelib::do_decompress(&input);
  assert_eq!(result, Err("Binary tree error: Type1".to_owned()))
}
