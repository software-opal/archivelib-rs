fn hex(data: &str) -> Box<[u8]> {
  let cleaned: Vec<u32> = data.chars().filter_map(|c| c.to_digit(16)).collect();
  assert!(cleaned.len() % 2 == 0);
  cleaned
    .chunks(2)
    .map(|dat| ((dat[0] << 4) + dat[1]) as u8)
    .collect::<Vec<_>>()
    .into_boxed_slice()
}
macro_rules! from_iter {
  ($iter: expr) => {
    $iter.collect::<Vec<_>>().into_boxed_slice()
  };
  ($($iter: expr),+) => {
    from_iter!($($iter,)+);
  };
  ($($iter: expr, )+) => {{
    let mut data: Vec<u8> = Vec::new();
    $(
    data.extend($iter);
    )+
    data.into_boxed_slice()
  }};
}

fn _bytes_to_human_hex(expected: &[u8], len: usize) -> Vec<String> {
  let mut b = expected
    .iter()
    .map(|b| format!("{:02X}", b))
    .collect::<Vec<_>>();
  while b.len() < len {
    b.push("~~".to_string());
  }
  let chunks = b.chunks(8).map(|s| s.join(" ")).collect::<Vec<_>>();
  chunks.chunks(2).map(|s| s.join("  ")).collect()
}

fn assert_bytes_eq(expected: &[u8], actual: &[u8]) {
  if expected == actual {
    return;
  }
  let len = expected.len().max(actual.len());
  let expected_bytes = _bytes_to_human_hex(expected, len);
  let actual_bytes = _bytes_to_human_hex(actual, len);
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
}

macro_rules! test_data {
  ($($name: ident => (in=$uncompressed_data:expr, out=$compressed_data:expr),)*) => {
    $(
      mod $name {
        #[allow(unused_imports)]
        use std::iter::repeat;

        use crate::{do_compress, do_decompress};
        #[allow(unused_imports)]
        use crate::test::fixed::*;
        #[allow(unused_imports)]
        use super::*;

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

mod real_world;
mod simple;
mod work_fn224;

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
  ),
}
