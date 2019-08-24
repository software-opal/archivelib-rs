/// The C library handles end of file in a somewhat confusing way. The library will repeatedly
/// read the last 8 bits of a file until the parent loop has read past the end of file enough times

macro_rules! check {
  ($($name: ident($input: expr, $output: expr));+ )=> {
    $(
      #[test]
      fn $name() {
        let input = $input;
        let expected = $output;

        // Sanity check the input and output;
        assert_eq!(
          archivelib_sys::do_decompress(input).unwrap(),
          expected,
          "System library doesn't match expected result."
        );
        assert_eq!(
          archivelib::do_decompress(input).unwrap(),
          expected,
          "Rust library fails for input: {:?}",
          input
        );
      }
    )+
  };
}


check!(
  // SHA1: da39a3ee5e6b4b0d3255bfef95601890afd80709
  empty_file([], [0x00]);
  // SHA1: 6c87e8951299d8a532146a93911048146b6fe1e0
  short_file_00_03([0x00, 0x03], [0x00]);
)
