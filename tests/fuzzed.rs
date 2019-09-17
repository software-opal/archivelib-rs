#[macro_use]
mod utils;

test_match_sys_decompress! {
  short_sample => hex!("1a 1a"),
  // This test crash was caused by adding two u16s together in fn258 which overflowed.
  fn258_crash => hex!("
    01 00 00 00 03 00 00 ff  2a ff ff 00 00 8c 00 00
    00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00  ff 00 ff 00 00 00 00 00
    00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
    00 00 ff ff ff 6d 00 00  00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00  ff 00 00 00 00 00 00 00
    00 00 ff ff ff 00 00 00  00 00 00 00 0a b4 02 00
    00 6e 05 ff 3f b6 e9 4a  2e 36 3d e5 e2 fa fd 17
    e5 2b 0a 08 68 e5 30 2b  ff a2 ff 24 df ff 00 1a
    1d 05 0a 1f 0a 5d 05 00  00 05 5a 08 22 e5 1c 41
    28 80 21 02 3b 2b 41 0a  41 a2 17 00 20 e5 be f5
    3f 0a 05 ff ff ff fe ff  ff ff 06 06 ff ff 00 2b
    14 14 14 14 c5 a2 5d 00  01 00 00 00 00 00 07 9b
    00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 10
    00 00 00 00 00 ff ff ff  ff ff 25 ff ff ff fb 1b
    ff 39 08 e4 76 a2 6f 00  5d 5d 58 00 00 00 80 21
    02 3b 00 00 e5 2b ff ff  7f bb 8a c0 f5 c0 49 00
    04 00 c4 3b 5d 1a 1d 41  0a 05 00 06 6e d8 01 1a
    41 d8 01 1a b6 8a 5d b6  44 44 44 13 54 7a 21 02
    3b a8 05 51 ff 00 0a 00  e5 2b 0a 08 9e 68 e5 6b
    1c 6e 05 5a 08 31 00 00  51 c5 a2 f8 97 1a de 00
    00 00 4d f7 40 1d 2b 0a  3b ff 05 00 ff d5 c9 c2
    1a 1d 40 ff df 1a 41 0a  3f f6 f3 02 c2 ff f5 3f
    0a ff 00 40 00 00 00 00  00 3f b6 fb 1b 93 02 68
    ff ff 3f 00 00 00 3b 00  ff ff ff df ff ff ff 5d
    b6 44 44 05 44 13 ff ff  5d b6 44 44 05 44 13 f8
    97 1a de 00 00 ff ff ff  ff 44 44 05 44 13 3a 54
    a3 ff 39 08 e4 76 a2 6f  00 5d 5d 58 00 00 00 1b
    93 02 68 00 00 e5 6b 0a  00 05 ff ff ff fe ff ff
    ff 06 06 ff ff ff 1a 1d  05 02 00 2b 41 fb 1b 93
    02 68 e5 2b 0a 08 9e 68  e5 6b 1c 6e 05 5a 08 34
    00 00 51 2c a2 f8 97 12  00 00 00 00 00 00 00 ff
    00 00 00 00 00 00 00 00  00 ff ff 00 01 00 ff ff
    ff 00 00 ff ff ff ff 00  00 00
  "),
  sample => hex!("00 03 20 04 3F F0 1A E7  C0 02"),
}

test_data! {
  t00 => (
    in=hex!("00"),
    out=hex!("00 02 20 04 3F FB D3 00  10")
  ),
  t01 => (
    in=hex!("01"),
    out=hex!("00 02 22 08 3F F9 FA 00  02")
  ),
}
