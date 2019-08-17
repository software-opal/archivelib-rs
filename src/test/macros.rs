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
#[cfg(test)]
macro_rules! from_iter {
  ($iter: expr) => {
    $iter.collect::<std::vec::Vec<_>>().into_boxed_slice()
  };
  ($($iter: expr),+) => {
    from_iter!($($iter,)+);
  };
  ($($iter: expr, )+) => {{
    let mut data: std::vec::Vec<u8> = Vec::new();
    $(
    data.extend($iter);
    )+
    data.into_boxed_slice()
  }};
}

#[macro_export]
#[cfg(test)]
macro_rules! rvec {
  ($($val: expr => $count: expr),+) => {{
      let mut v = Vec::new();
      $(
        v.resize(v.len() + $count, $val);
      )+
      v}
  };
}

#[macro_export]
#[cfg(test)]
macro_rules! test_data {
  ($($name: ident => (in=$uncompressed_data:expr, out=$compressed_data:expr),)*) => {
    $(
      pub mod $name {
        use crate::{do_compress, do_decompress};
        #[allow(unused_imports)]
        use super::*;

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
#[macro_export]
#[cfg(test)]
macro_rules! match_sys_test_data {
  ($($name: ident => $compressed_data:expr,)*) => {
    $(
      pub mod $name {
        #[allow(unused_imports)]
        use super::*;

        #[test]
        fn test_decompress() {
          let compressed = $compressed_data;
          check_rust_against_sys_decompress!(compressed);
        }
      }
    )*
  };
}

#[cfg(test)]
macro_rules! test_crash_case {
  ($($name: ident => $compressed_data:expr,)*) => {
    $(
      #[test]
      fn $name() {
        let compressed = $compressed_data;
        match crate::do_decompress(&compressed[..]) {
          Ok(data) => {
            // If we succeed; then the sys library should too
            // let expected2 = archivelib_sys2::do_decompress(&compressed[..]).unwrap();
            let expected = archivelib_sys::do_decompress(&compressed[..]).unwrap();
            assert_eq!(data, expected, "Decompress of {:X?} differed from sys.", &compressed[..]);
            // assert!(false, "Decompress should have failed on {:X?}, instead got {:X?}", &compressed[..], data);
          }
          Err(_) => {}
        }
      }
    )*
  };
}

#[macro_export]
#[cfg(test)]
macro_rules! fuzzer_test_data {
  ($($name: ident => $uncompressed_data:expr,)*) => {
    $(
      pub mod $name {
        // #[allow(unused_imports)]
        // use std::iter::repeat;
        // #[allow(unused_imports)]
        // use crate::test::fixed::*;

        use crate::{do_compress, do_decompress};
        #[allow(unused_imports)]
        use super::*;
        pub fn get_uncompressed() -> Box<[u8]> { $uncompressed_data }

        #[test]
        fn test_compression_port(vec in raw_data_strat(), level in level_strat()) {
          let uncompressed = get_uncompressed();
          let real_data = match do_compress(&uncompressed, level).unwrap();
          let test_data = match do_ported_compress(&uncompressed, level).unwrap();
          assert_eq!(real_data, test_data, "Compression produced different results");
        }
        #[test]
        fn test_decompression_port(vec in raw_data_strat(), level in level_strat()) {
          let uncompressed = get_uncompressed();
          let data = match do_compress(&uncompressed).unwrap();
          let result = match do_ported_decompress_level(&data);
          assert_eq!(&uncompressed[..], &result[..], "Data is not identical after decompression");
        }
      }
    )*
  };
}

#[macro_export]
#[cfg(test)]
macro_rules! test_compare_sys {
  ($($name:ident = $data:expr),*) => {
    $(
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
    )*
  };
}
