test_data!{
  case_for_nonzero_arg =>(
    in=from_iter!(
      &[0x88, 0x81, 0x81],
      &[0x88, 0x81, 0x81],
    ),
    out=hex(     "00 05 30 64 6D FF C6 DB  0F 8C 78 E9 E3 20 E2 B8"    )
  ),
  case2_for_nonzero_arg =>(
    in=from_iter!(
      &[0x88, 0x81, 0x81, 0x81],
      &[0x88, 0x81, 0x81],
    ),
    out=hex("00 06 30 64 6D FF C6 DB  0F 8C 78 E9 E3 20 E1 7C")
  ),
}
