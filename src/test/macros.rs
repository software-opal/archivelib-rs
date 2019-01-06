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
macro_rules! rvec {
  ($($val: expr => $count: expr),+) => {{
      let mut v = Vec::new();
      $(
        v.resize(v.len() + $count, $val);
      )+
      v}
  };
}

macro_rules! _bytes_to_human_hex {
  ($expected: expr, $len: expr) => {{
    let expected: &[u8] = $expected;
    let len: usize = $len;
    let mut b = expected
      .iter()
      .map(|b| format!("{:02X}", b))
      .collect::<Vec<_>>();
    while b.len() < len {
      b.push("~~".to_string());
    }
    b.chunks(32)
      .map(|s| {
        s.chunks(8)
          .map(|s| s.join(" "))
          .collect::<Vec<_>>()
          .join("  ")
      })
      .collect::<Vec<_>>()
  }};
}

#[macro_export]
macro_rules! assert_bytes_eq {
  ($expected: expr, $actual: expr) => {{
    let expected: &[u8] = $expected;
    let actual: &[u8] = $actual;
    if expected == actual {
      return;
    }
    let len = expected.len().max(actual.len());
    let expected_bytes = _bytes_to_human_hex!(expected, len);
    let actual_bytes = _bytes_to_human_hex!(actual, len);
    let mut data: Vec<String> = Vec::with_capacity(len);
    let mut diffs = Vec::new();
    let mut has_more = false;
    for (idx, (ref expected_r, ref actual_r)) in
      expected_bytes.iter().zip(actual_bytes.iter()).enumerate()
    {
      if expected_r != actual_r {
        if diffs.len() < 10 {
          diffs.push(idx);
          if idx > 0 {
            diffs.push(idx - 1);
          }
          if idx + 1 < data.len() {
            diffs.push(idx + 1);
          }
          diffs.sort();
          diffs.dedup();
        } else {
          has_more = true;
        }
      }
      data.push(
        expected_r
          .chars()
          .zip(actual_r.chars())
          .map(|(e, r)| if e == r { "─" } else { "┴" })
          .collect(),
      );
    }
    diffs.sort();
    diffs.dedup();
    let mut out = "\n".to_string();
    let mut last = 0;
    for row in diffs {
      let expected_r = &expected_bytes[row];
      let actual_r = &actual_bytes[row];
      let note_r = &data[row];
      if row > 0 && last != row - 1 {
        out.push_str(&format!(
          " ... {} equal rows skipped ...\n",
          (row - last - 1)
        ));
      }
      out.push_str(&format!("      ╭╴Expected: {}\n", expected_r));
      out.push_str(&format!("{:>5}╺┽──╴Actual: {}\n", row, actual_r));
      out.push_str(&format!(
        "      ╰───────────{}\n",
        note_r
      ));
      last = row;
    }
    if has_more {
      out.push_str(&format!(
        " ... {} more rows not shown ...\n",
        expected_bytes.len() - last - 1,
      ));
    } else if last + 1 != expected_bytes.len() {
      out.push_str(&format!(
        " ... {} equal rows skipped ...\n",
        expected_bytes.len() - last - 1,
      ));
    }

    assert_eq!(expected, actual, "{}", out);
  }};
}

#[macro_export]
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
macro_rules! fuzzer_test_data {
  ($($name: ident => $uncompressed_data:expr,)*) => {
    $(
      pub mod $name {
        #[allow(unused_imports)]
        use std::iter::repeat;

        use crate::{do_compress, do_decompress};
        #[allow(unused_imports)]
        use crate::test::fixed::*;
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
