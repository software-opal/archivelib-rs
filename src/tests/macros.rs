macro_rules! test_compare_sys {
  ($name:ident = $data:expr) => {
    mod $name {
      fn get_data() -> Box<[u8]> {
        $data
      }

      #[test]
      fn test_compress() {
        let data = get_data();
        let compressed_sample = archivelib_sys::do_compress(&data).unwrap();
        let compressed_test = crate::do_compress(&data).unwrap();
        assert_eq!(compressed_sample[..], compressed_test[..]);
      }

      #[test]
      fn test_decompress() {
        let data = get_data();
        let compressed = archivelib_sys::do_compress(&data).unwrap();
        println!("input = {:X?}", compressed);
        let decompressed = crate::do_decompress(&compressed).unwrap();
        assert_eq!(decompressed[..], data[..]);
      }
    }
  };
}
