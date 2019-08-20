#[macro_export]
macro_rules! test_match_sys_decompress {
  ($($name: ident => $compressed_data:expr,)*) => {
    $(
      pub mod $name {
        #[test]
        fn test_decompress() {
          let compressed = $compressed_data;
          check_rust_against_sys_decompress!(compressed);
        }
      }
    )*
  };
}

#[macro_export]
macro_rules! hex {
  ($data: expr) => {{
    let cleaned: std::vec::Vec<u32> = $data.chars().filter_map(|c| c.to_digit(16)).collect();
    assert!(cleaned.len() % 2 == 0);
    cleaned
      .chunks(2)
      .map(|dat| ((dat[0] << 4) + dat[1]) as u8)
      .collect::<std::vec::Vec<_>>()
      .into_boxed_slice()
  }};
}

#[macro_export]
macro_rules! test_data {
  ($($name: ident => (in=$uncompressed_data:expr, out=$compressed_data:expr),)*) => {
    $(
      pub mod $name {
        use archivelib::{do_compress, do_decompress};

        pub fn get_uncompressed() -> Box<[u8]> { $uncompressed_data }
        pub fn get_compressed() -> Box<[u8]> { $compressed_data }

        #[test]
        fn test_compress() {
          let uncompressed = get_uncompressed();
          let compressed = get_compressed();
          let compress_output = do_compress(&uncompressed[..]).unwrap();
          assert_bytes_eq!(&compressed[..], &compress_output);
        }
        #[test]
        fn test_decompress() {
          let uncompressed = get_uncompressed();
          let compressed = get_compressed();
          let decompress_output = do_decompress(&compressed[..]).unwrap();
          assert_bytes_eq!(&uncompressed[..], &decompress_output);
        }
      }
    )*
  };
}
