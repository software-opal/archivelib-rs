#[macro_use]
mod macros;
test_match_sys_decompress! {
  data => hex!("
      00 18 40 68 61 B5 FF 0D  9F 43 D1 10 BC A0 CB 89  DA 80 16 77 00 01 B6 7B  39 F0
  "),
}
