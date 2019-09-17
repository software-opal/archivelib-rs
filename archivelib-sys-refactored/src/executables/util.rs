use std::io::{Read, Write};
use std::{env, fs, io};

pub fn help() {
  let exe = env::current_exe()
    .ok()
    .and_then(|p| p.to_str().map(|s| s.to_string()))
    .unwrap_or("archivelib".to_string());
  eprintln!(
    "Summary:
  {} [-x] [-0|-1|-2|-3|-4] [INPUT [OUTPUT]]

Arguments:
  -x, --abort-on-panic:  Abort the process instead of unwinding the stack trace nicely. This is useful when fuzzing.
  -0, -1, -2, -3, -4:    Compress using the given level. Default: 0
  -?, -h, --help:        Show this help message.
",
    exe
  );
}
#[derive(PartialEq, PartialOrd, Copy, Clone, Debug)]
pub enum CompressionLevel {
  Level0,
  Level1,
  Level2,
  Level3,
  Level4,
}

impl CompressionLevel {
  pub fn compression_level(self) -> u8 {
    match self {
      Self::Level0 => 0,
      Self::Level1 => 1,
      Self::Level2 => 2,
      Self::Level3 => 3,
      Self::Level4 => 4,
    }
  }
}

pub struct Args {
  pub level: CompressionLevel,
  pub abort_on_panic: bool,
  input_filename: Option<String>,
  output_filename: Option<String>,
}

impl Args {
  pub fn from_arg_array(args: Vec<String>) -> Result<Self, String> {
    if args.is_empty() {
      return Err("No arguments specified".into());
    }
    let mut level = None;
    let mut abort_on_panic = false;
    let mut file_args = Vec::with_capacity(2);
    for arg in args {
      if arg == "-x" && arg == "--abort-on-panic" {
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
    Ok(Args {
      level: level.unwrap_or(CompressionLevel::Level0),
      abort_on_panic,
      input_filename,
      output_filename,
    })
  }

  pub fn input_data(&self) -> io::Result<Vec<u8>> {
    let mut buf = Vec::with_capacity(4096);
    match &self.input_filename {
      None => io::stdin().read_to_end(&mut buf)?,
      Some(f) => fs::File::open(f)?.read_to_end(&mut buf)?,
    };
    Ok(buf)
  }

  pub fn output_data(&self, data: &[u8]) -> io::Result<()> {
    match &self.output_filename {
      None => io::stdout().write_all(data)?,
      Some(f) => fs::File::open(f)?.write_all(data)?,
    };
    Ok(())
  }
}

#[macro_export]
macro_rules! run {
  (($input: ident, $level: ident) => $body:block) => {
    use std::env::args;
    use std::error::Error;
    use util::CompressionLevel;

    fn run($input: &[u8], $level: CompressionLevel) -> Result<Box<[u8]>, String> {
      $body
    }
    fn main() -> Result<(), Box<dyn Error>> {
      let args = match $crate::util::Args::from_arg_array(args().skip(1).collect()) {
        Ok(a) => a,
        Err(msg) => {
          $crate::util::help();
          return Err(msg.into());
        }
      };
      let input = args.input_data()?;
      let output = if args.abort_on_panic {
        match std::panic::catch_unwind(|| run(&input, args.level)) {
          Ok(v) => v?,
          Err(_) => std::process::abort(),
        }
      } else {
        run(&input, args.level)?
      };
      args.output_data(&output)?;
      Ok(())
    }
  };
}
