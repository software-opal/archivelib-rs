#[macro_use]
mod macros;
test_match_sys_decompress! {
  data => hex!("
      00 12 43 88 81 A7 FF 0D  9A C8 F4 61 B4 81 94 00  20 9B D4 90 00 00 19 3C  00 62 A5 C1 81 AF F0
  "),
}
