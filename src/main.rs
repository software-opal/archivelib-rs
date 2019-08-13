#[cfg(all(not(feature = "fuzz-afl"), feature = "fuzz-hfuzz"))]
#[macro_use]
extern crate honggfuzz;

#[cfg(all(feature = "fuzz-afl", not(feature = "fuzz-hfuzz")))]
#[macro_use]
extern crate afl;

use archivelib::CompressionLevel;
use std::error::Error;
use std::io::Read;
use std::io::Write;
use std::{env, fs, io};

fn help() {
  let exe = env::current_exe()
    .ok()
    .and_then(|p| p.to_str().map(|s| s.to_string()))
    .unwrap_or("archivelib".to_string());
  eprintln!(
    "Summary:
  {} (-c|--compress|-d|--decompress) [-x] [-0|-1|-2|-3|-4] [INPUT [OUTPUT]]

Arguments:
  -c, --compress:        Perform compression.
  -d, --decompress:      Perform decompression.
  -x, --abort-on-panic:  Abort the process instead of unwinding the stack trace nicely. This is useful when fuzzing.
  -0, -1, -2, -3, -4:    Compress using the given level. Default: 0
  -?, -h, --help:        Show this help message.
",
    exe
  );
}

#[derive(Copy, Clone, Debug)]
enum Mode {
  COMPRESS,
  DECOMPRESS,
}

struct Args {
  mode: Mode,
  level: CompressionLevel,
  abort_on_panic: bool,
  input_filename: Option<String>,
  output_filename: Option<String>,
}

impl Args {
  fn from_arg_array(args: Vec<String>) -> Result<Self, Box<dyn Error>> {
    if args.is_empty() {
      return Err("No arguments specified".into());
    }
    let mut mode = None;
    let mut level = None;
    let mut abort_on_panic = false;
    let mut file_args = Vec::with_capacity(2);
    for arg in args {
      if arg == "-c" || arg == "--compress" {
        mode = Some(Mode::COMPRESS)
      } else if arg == "-d" || arg == "--decompress" {
        mode = Some(Mode::DECOMPRESS)
      } else if arg == "-x" && arg == "--abort-on-panic" {
        abort_on_panic = true;
      } else if arg == "-0" {
        level = Some(CompressionLevel::Level0);
      } else if arg == "-1" {
        level = Some(CompressionLevel::Level1);
      } else if arg == "-2" {
        level = Some(CompressionLevel::Level2);
      } else if arg == "-3" {
        level = Some(CompressionLevel::Level3);
      } else if arg == "-4" {
        level = Some(CompressionLevel::Level4);
      } else if arg == "-?" || arg == "-h" || arg == "--help" {
        return Err("".into());
      } else {
        file_args.push(arg)
      }
    }
    let (input_filename, output_filename) = {
      let mut f = file_args.into_iter();
      let input_filename = f.next();
      let output_filename = f.next();
      if let Some(arg) = f.next() {
        return Err(
          format!(
            "Too many file arguments. You can specify up to 2 files: {:?}, {:?}, {:?}",
            input_filename, output_filename, arg
          )
          .into(),
        );
      }
      (input_filename, output_filename)
    };
    let mode = match mode {
      None => {
        return Err(format!("You must specify one of --compress or --decompress!").into());
      }
      Some(m) => m,
    };
    let level = level.unwrap_or(CompressionLevel::Level0);
    Ok(Args {
      mode,
      level,
      abort_on_panic,
      input_filename,
      output_filename,
    })
  }

  fn input_data(&self) -> Result<Vec<u8>, Box<dyn Error>> {
    let mut buf = Vec::with_capacity(4096);
    match &self.input_filename {
      None => io::stdin().read_to_end(&mut buf)?,
      Some(f) => fs::File::open(f)?.read_to_end(&mut buf)?,
    };
    Ok(buf)
  }

  fn output_data(&self, data: &[u8]) -> Result<(), Box<dyn Error>> {
    match &self.output_filename {
      None => io::stdout().write_all(data)?,
      Some(f) => fs::File::open(f)?.write_all(data)?,
    };
    Ok(())
  }
}
fn run(input: &[u8], mode: Mode, level: CompressionLevel) -> Result<Box<[u8]>, Box<dyn Error>> {
  Ok(match mode {
    Mode::COMPRESS => archivelib::do_compress_level(&input, level.compression_level())?,
    Mode::DECOMPRESS => archivelib::do_decompress_level(&input, level.compression_level())?,
  })
}

#[cfg(all(not(feature = "fuzz-afl"), not(feature = "fuzz-hfuzz")))]
fn main() -> Result<(), Box<dyn Error>> {
  let args = match Args::from_arg_array(env::args().skip(1).collect()) {
    Ok(a) => a,
    Err(msg) => {
      help();
      return Err(msg);
    }
  };

  let input = args.input_data()?;
  let output = if args.abort_on_panic {
    match std::panic::catch_unwind(|| run(&input, args.mode, args.level)) {
      Ok(v) => v?,
      Err(_) => std::process::abort(),
    }
  } else {
    run(&input, args.mode, args.level)?
  };
  args.output_data(&output)
}

#[cfg(all(feature = "fuzz-afl", not(feature = "fuzz-hfuzz")))]
fn main() -> Result<(), Box<dyn Error>> {
  let args = match Args::from_arg_array(env::args().skip(1).collect()) {
    Ok(a) => a,
    Err(msg) => {
      help();
      return Err(msg);
    }
  };
  afl::fuzz!(|input| {
    run(input, args.mode, args.level);
  });
  Ok(())
}

#[cfg(all(not(feature = "fuzz-afl"), feature = "fuzz-hfuzz"))]
fn main() -> Result<(), Box<dyn Error>> {
  let args = match Args::from_arg_array(env::args().skip(1).collect()) {
    Ok(a) => a,
    Err(msg) => {
      help();
      return Err(msg);
    }
  };
  loop {
    honggfuzz::fuzz!(|input| {
      run(input, args.mode, args.level);
    })
  }
}
