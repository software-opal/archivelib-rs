use archivelib::CompressionLevel;
use std::error::Error;
use std::io::Read;
use std::io::Write;
use std::{env, fs, io};

fn help() {
  let exe = env::current_exe()
    .ok()
    .and_then(|p| p.to_str().map(|s| s.to_string()))
    .unwrap_or_else(|| "archivelib".to_string());
  eprintln!(
    "Summary:
  {} (-c|--compress|-d|--decompress) [-x] [-0|-1|-2|-3|-4] [INPUT [OUTPUT]]

Arguments:
  -c, --compress:        Perform compression.
  -d, --decompress:      Perform decompression.
  -V, --validate:        Validate the correctness of the algorithm by compressing, decompressing, and checking for equality.
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
  VALIDATE,
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
    let mut has_seen_dash_dash = false;
    for arg in args {
      if has_seen_dash_dash {
        file_args.push(arg);
      } else if arg == "-c" || arg == "--compress" {
        mode = Some(Mode::COMPRESS)
      } else if arg == "-d" || arg == "--decompress" {
        mode = Some(Mode::DECOMPRESS)
      } else if arg == "-V" || arg == "--validate" {
        mode = Some(Mode::VALIDATE)
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
      } else if arg == "--" {
        has_seen_dash_dash = true
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
        return Err("You must specify one of --compress or --decompress!".into());
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
      None => {
        io::stdout().write_all(data)?;
        io::stdout().flush()?;
      }
      Some(f) => {
        let mut file = fs::File::open(f)?;
        file.write_all(data)?;
        file.flush()?;
      }
    };
    Ok(())
  }
}
fn run(input: &[u8], mode: Mode, level: CompressionLevel) -> Result<Box<[u8]>, Box<dyn Error>> {
  match mode {
    Mode::COMPRESS => {
      let result = archivelib::do_compress_level(&input, level)?;
      Ok(result)
    }
    Mode::DECOMPRESS => {
      let result = archivelib::do_decompress_level(&input, level)?;
      Ok(result)
    }
    Mode::VALIDATE => {
      let compressed = archivelib::do_compress_level(&input, level)?;
      let result = archivelib::do_decompress_level(&compressed, level)?;
      if input == &result[..] {
        Ok(result)
      } else {
        let input_len = input.len();
        let output_len = input.len();

        Err(
          format!(
            "Data mismatch between input({} characters) and output({} characters)",
            input_len, output_len
          )
          .into(),
        )
      }
    }
  }
}

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
