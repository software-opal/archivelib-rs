macro_rules! fuzz_with_main {
  ($fn: ident) => {
    #[cfg(fuzzing)]
    #[macro_use]
    extern crate libfuzzer_sys;

    #[cfg(fuzzing)]
    fuzz_target!(|data: &[u8]| {
      $fn(data);
    });

    #[cfg(not(fuzzing))]
    fn main() {
      use std::fs::File;
      use std::io::Read;

      let mut buffer = vec![];
      File::open(std::env::args().nth(1).unwrap())
        .unwrap()
        .read_to_end(&mut buffer)
        .unwrap();
      $fn(&buffer);
    }
  };
  (|$data:ident: &[u8]| $body:expr) => {
    fn target($data: &[u8]) {
      $body
    }
    fuzz_with_main!(target);
  };
}
