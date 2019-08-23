#[macro_use]
mod macros;
test_match_sys_decompress! {
  data => hex!("
      00 0D 38 69 69 7F C3 65  F5 A1 6E 06 92 1D 50 28  CB 20 00 02 01 67 00 71  F5 51 BC 98 5E
  "),
}
