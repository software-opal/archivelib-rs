use std::time::Duration;

use archivelib::CompressionLevel;

const TEST_TIMEOUT: Duration = Duration::from_secs(5);

fn run_testcase(
  data: Box<[u8]>,
  level: CompressionLevel,
) -> std::result::Result<std::boxed::Box<[u8]>, std::string::String> {
  let thread = humthreads::Builder::new("test thread").spawn( move |_| {
    archivelib::check_rust_against_sys_decompress!(archivelib_sys::do_decompress_level; &data[..], level)
  }).unwrap();
  let result = thread.join_timeout(TEST_TIMEOUT);
  match result {
    Ok(r) => r,
    Err(e) => match e.kind() {
      humthreads::ErrorKind::JoinTimeout => panic!("Test failed to complete in {:?}", TEST_TIMEOUT),
      humthreads::ErrorKind::Join(_) => {
        panic!("Test panicked; can't seem to access why so ... good luck!?")
      }
      _ => panic!("Another thread error"),
    },
  }
}

macro_rules! test_hang {
  ($($(#[$attr:meta])* $name: ident($input: expr_2021);)+ )=> {
    $(
      #[test]
      $(#[$attr])*
      fn $name() {
        let input: Box<[u8]> = Box::new($input);
        // Ignore the result; because we don't care if the code errored; just that it didn't hang
        let _ = run_testcase(input, CompressionLevel::Level0);
      }
    )+
  };
}

test_hang! {
  #[ignore = "New implementation has different bugs"]
  test_short_file_ff_5f([0xFF,  0x5F]);
}
