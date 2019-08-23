#[macro_use]
mod macros;
test_match_sys_decompress! {
  data => hex!("
      FF 2F
  "),
}
