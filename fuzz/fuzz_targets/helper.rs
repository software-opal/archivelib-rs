macro_rules! fuzz_with_main {
  (|$data:ident: &[u8]| $body:expr) => {
    #[cfg(fuzzing)]
    #[macro_use]
    extern crate libfuzzer_sys;

    #[cfg(fuzzing)]
    fuzz_target!(|data: &[u8]| {
      target(data);
    });

    fn target($data: &[u8]) {
      $body
    }

    #[cfg(not(fuzzing))]
    fn main() {
      use std::fs::File;
      use std::io::Read;

      let mut buffer = vec![];
      File::open(std::env::args().nth(1).unwrap())
        .unwrap()
        .read_to_end(&mut buffer)
        .unwrap();
      target(&buffer);
    }
  };
}
