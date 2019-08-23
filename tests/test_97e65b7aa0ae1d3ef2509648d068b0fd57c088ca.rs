#[macro_use]
mod macros;
test_match_sys_decompress! {
  data => hex!("
      08 FF FF 73
  "),
}
