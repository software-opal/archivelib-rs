mod binary_tree_printer;

#[macro_export]
macro_rules! check_rust_against_sys_decompress {
  ($compressed: expr_2021) => {{
    $crate::check_rust_against_sys_decompress!($crate::sys::do_decompress_level; $compressed, CompressionLevel::Level0)
  }};
  ($sys_decompress: path; $compressed: expr_2021) => {{
    $crate::check_rust_against_sys_decompress!($sys_crate; $compressed, CompressionLevel::Level0)
  }};
  ($compressed: expr_2021, $level: expr_2021) => {{
    $crate::check_rust_against_sys_decompress!($crate::sys::do_decompress_level; $compressed, $level)
  }};
  ($sys_decompress: path; $compressed: expr_2021, $level: expr_2021) => {{
    use $crate::CompressionLevel;
    let compressed = $compressed;
    let level: CompressionLevel = $level;
    match $crate::do_decompress_level(&compressed[..], level) {
      Ok(decompress_output) => {
        match $sys_decompress(&compressed[..], level.compression_level()) {
          Ok(sys_output) => {
            $crate::assert_bytes_eq!(&sys_output, &decompress_output[..]);
          }
          Err(err) => {
            assert!(false, "System library failed with error; but rust library succeeded. Error: {}", err);
          }
        }
        Ok(decompress_output)
      }
      Err(msg) => {
        if msg == "Binary tree error: Type1" || msg == "Binary tree error: Type1" ||msg == "File Exhausted" {
          match $sys_decompress(&compressed[..], level.compression_level()) {
            Ok(_sys_output) => {
              panic!("archivelib::do_decompress failed with a binary tree error({}); but the system library succeeded!", msg)
            }
            Err(err) => {
              match msg.as_str() {
                "Binary tree error: Type1" => {
                  if err.ends_with("Internal 1 error in Greenleaf Decompression routine\u{0}") {}
                  else if err == "Internal error: -101\0" {}
                  else {
                    panic!("Rust library failed with {:?}; but system library failed with a different error {:?}", msg, err)
                  }
                },
                "Binary tree error: Type2" => {
                  if err.ends_with("Internal 2 error in Greenleaf Decompression routine\u{0}") {}
                  else if err == "Internal error: -102\0" {}
                  else {
                    panic!("Rust library failed with {:?}; but system library failed with a different error {:?}", msg, err)
                  }
                },
                "File Exhausted" => {
                  unimplemented!("File exhausted!");
                }
                _ =>{ unreachable!("Message was not recognised: {}", msg)}
              }
            }
          }
        } else if msg == "Invariant Failure" {
          // These usually crash the system library; so just *assume* the input is fine
        } else {
          panic!("archivelib::do_decompress failed with an unexpected error: {}", msg);
        }
        Err(msg)
      }
    }
  }};
}

#[macro_export]
macro_rules! assert_bytes_eq {
  ($expected: expr_2021, $actual: expr_2021) => {{
    let orig_expected = $expected;
    let orig_actual = $actual;
    let expected = orig_expected.iter().collect::<Vec<_>>();
    let actual = orig_actual.iter().collect::<Vec<_>>();
    if expected == actual {
      assert_eq!(expected, actual, "Sanity check failed");
    } else {
      let len = expected.len().max(actual.len());
      let expected_bytes = $crate::_bytes_to_human_hex!(expected, len);
      let actual_bytes = $crate::_bytes_to_human_hex!(actual, len);
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
            .map(|(e, r)| if e == r { "\u{2500}" } else { "\u{2534}" })
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
        out.push_str(&format!("      ╰───────────{}\n", note_r));
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

      // assert_eq!(
      //   expected,
      //   actual,
      panic!(
        "Expected length: {}, Actual length: {}{}",
        expected.len(),
        actual.len(),
        out
      );
    }
  }};
}

#[macro_export]
macro_rules! _bytes_to_human_hex {
  ($expected: expr_2021, $len: expr_2021) => {{
    let expected = $expected.clone();
    let len: usize = $len;
    let test = expected.get(0).unwrap_or(&&0);
    let bitsize = (test.count_zeros() + test.count_ones()) as usize / 4;
    let mut b = expected
      .iter()
      .map(|&b| format!("{:01$X}", b, bitsize,))
      .collect::<Vec<_>>();
    if b.len() < len {
      let pad_string = "~".chars().cycle().take(bitsize).collect::<String>();
      while b.len() < len {
        b.push(pad_string.clone());
      }
    };
    // u8 -> 32; u16 -> 32; u32 -> 16; u64/usize -> 8;
    let chunk_size = (128 / bitsize).min(32);
    let spacer_size = if bitsize > 8 { 4 } else { 8 };
    b.chunks(chunk_size)
      .map(|s| {
        s.chunks(spacer_size)
          .map(|s| s.join(" "))
          .collect::<Vec<_>>()
          .join("  ")
      })
      .collect::<Vec<_>>()
  }};
}
