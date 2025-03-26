#[macro_export]
macro_rules! test_match_sys_decompress {
  ($($name: ident => $compressed_data:expr_2021,)*) => {
    $(
      pub mod $name {
        lazy_static::lazy_static! {
          static ref DATA: Vec<u8> = {
            let data = $compressed_data;
            data.to_vec()
          };
        }

        #[test]
        fn test_decompress_level_0() {
          let _ = archivelib::check_rust_against_sys_decompress!(archivelib_sys::do_decompress_level; &DATA[..], CompressionLevel::Level0);
        }
        #[test]
        fn test_decompress_level_1() {
          let _ = archivelib::check_rust_against_sys_decompress!(archivelib_sys::do_decompress_level; &DATA[..], CompressionLevel::Level1);
        }
        #[test]
        fn test_decompress_level_2() {
          let _ = archivelib::check_rust_against_sys_decompress!(archivelib_sys::do_decompress_level; &DATA[..], CompressionLevel::Level2);
        }
        #[test]
        fn test_decompress_level_3() {
          let _ = archivelib::check_rust_against_sys_decompress!(archivelib_sys::do_decompress_level; &DATA[..], CompressionLevel::Level3);
        }
        #[test]
        fn test_decompress_level_4() {
          let _ = archivelib::check_rust_against_sys_decompress!(archivelib_sys::do_decompress_level; &DATA[..], CompressionLevel::Level4);
        }
      }
    )*
  };
}

#[macro_export]
macro_rules! hex {
  ($data: expr_2021) => {{
    let cleaned: std::vec::Vec<u32> = $data.chars().filter_map(|c| c.to_digit(16)).collect();
    assert!(cleaned.len() % 2 == 0);
    cleaned
      .chunks(2)
      .map(|dat| ((dat[0] << 4) + dat[1]) as u8)
      .collect::<std::vec::Vec<_>>()
  }};
}
#[macro_export]
macro_rules! binary {
  ($data: expr_2021) => {
    $data
      .chars()
      .filter_map(|c| match c {
        '0' => false,
        '1' => true,
        _ => panic!("invalid character {:?}", c),
      })
      .collect()
  };
}

#[macro_export]
macro_rules! test_data {
  ($($name: ident => (in=$uncompressed_data:expr_2021, out=$compressed_data:expr_2021),)*) => {
    $(
      pub mod $name {
        use archivelib::{do_compress, do_decompress};
        lazy_static::lazy_static! {
          static ref COMPRESSED: Vec<u8> = {
            let data = $compressed_data;
            data.to_vec()
          };
          static ref UNCOMPRESSED: Vec<u8> = {
            let data =$uncompressed_data;
            data.to_vec()
          };
        }

        #[test]
        fn test_compress() {
          let compress_output = do_compress(&UNCOMPRESSED[..]).unwrap();
          archivelib::assert_bytes_eq!(&COMPRESSED[..], &compress_output);
        }
        #[test]
        fn test_decompress() {
          let decompress_output = do_decompress(&COMPRESSED[..]).unwrap();
          archivelib::assert_bytes_eq!(&UNCOMPRESSED[..], &decompress_output);
        }
      }
    )*
  };
}

#[macro_export]
macro_rules! check_decompress_matches {
  ($($name: ident($input: expr_2021, $output: expr_2021);)+ )=> {
    $(
      #[test]
      fn $name() {
        let input: &[u8] = &$input;
        let expected = $output;

        // Sanity check the input and output;
        assert_eq!(
          &expected[..],
          &archivelib_sys::do_decompress(&input[..]).unwrap()[..]
          // "System library doesn't match expected result."
        );
        archivelib::assert_bytes_eq!(
          &expected[..],
          &archivelib::do_decompress(&input[..]).unwrap()[..]
          // "Rust library fails for input: {:?}",
          // input
        );
      }
    )+
  };
}
