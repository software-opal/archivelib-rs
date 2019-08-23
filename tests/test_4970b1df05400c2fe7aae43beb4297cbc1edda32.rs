#[macro_use]
mod macros;
test_match_sys_decompress! {
  // Minified from crashes: id:000000,sig:06,src:000007,op:flip4,pos:1024
  data => hex!("
      30 30 77 76 F5 2A 4B BF  E0 08 51 52 AA B3 8C DF  15 BB AE 66 55 15 00 04  3D F7 82 CD DD DF B7 F7
      DF C3 BD F1 AB D5 DA E4  6E 36 CE
  "),
}
