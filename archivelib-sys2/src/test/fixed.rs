fn hex(data: &str) -> Box<[u8]> {
  let cleaned: Vec<u32> = data.chars().filter_map(|c| c.to_digit(16)).collect();
  assert!(cleaned.len() % 2 == 0);
  cleaned
    .chunks(2)
    .map(|dat| ((dat[0] << 4) + dat[1]) as u8)
    .collect::<Vec<_>>()
    .into_boxed_slice()
}

fn assert_bytes_eq(expected: &[u8], actual: &[u8]) {
  assert_eq!(expected, actual);
}

macro_rules! test_data {
  ($($name: ident => (in=$uncompressed_data:expr, out=$compressed_data:expr)),*) => {
    $(
      mod $name {
        use crate::{do_compress, do_decompress};
        use crate::test::fixed::*;

        #[test]
        fn test_compress() {
          let uncompressed = $uncompressed_data;
          let compressed = $compressed_data;
          let compress_output = do_compress(&uncompressed[..]).unwrap();
          assert_bytes_eq(&compressed[..], &compress_output);
        }
        #[test]
        fn test_decompress() {
          let uncompressed = $uncompressed_data;
          let compressed = $compressed_data;
          let decompress_output = do_decompress(&compressed[..]).unwrap();
          assert_bytes_eq(&uncompressed[..], &decompress_output);
        }
      }
    )*
  };
}

test_data!{
  round_trip => (
    in=b"what if this gets compressed well good!",
    out=vec![
      0, 40, 74, 14, 104, 47, 254, 6, 123, 10, 226, 124, 168, 48, 197, 208, 117, 202, 0, 54, 57,
      193, 61, 140, 137, 65, 206, 33, 213, 249, 239, 197, 34, 195, 77, 80, 123, 182, 227, 240
    ]
  ),
  round_trip2 => (
    in=b"what if this gets compressed well good!",
    out=hex(
      "00 28 4A 0E 68 2F FE 06  7B 0A E2 7C A8 30 C5 D0
       75 CA 00 36 39 C1 3D 8C  89 41 CE 21 D5 F9 EF C5
       22 C3 4D 50 7B B6 E3 F0"
    )
  )
}
