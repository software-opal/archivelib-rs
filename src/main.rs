use archivelib::CompressionLevel;
use std::error::Error;
use std::io::Read;
use std::io::Write;
use std::{env, fs, io};

enum Mode {
  COMPRESS,
  DECOMPRESS,
}

fn help() -> Result<(), Box<dyn Error>> {
  let exe = env::current_exe()
    .ok()
    .and_then(|p| p.to_str().map(|s| s.to_string()))
    .unwrap_or("archivelib-fuzz-afl".to_string());
  eprintln!(
    "Summary:
  {} (-c|--compress|-d|--decompress) [-0|-1|-2|-3|-4]

Arguments:
  -c, --compress:      Take stdin and compress it, outputting the compressed data to stdout.
  -d, --decompress:    Take stdin and decompress it, outputting the compressed data to stdout.
  -0, -1, -2, -3, -4:  Compress using the given level. Default: 0
  -?, -h, --help:      Show this help message.
",
    exe
  );
  return Err("".into());
}

fn input_file(arg: Option<&String>) -> Result<Vec<u8>, Box<dyn Error>> {
  let mut buf = Vec::with_capacity(4096);
  match arg {
    None => io::stdin().read_to_end(&mut buf)?,
    Some(f) => fs::File::open(f)?.read_to_end(&mut buf)?,
  };
  Ok(buf)
}

fn output_file(arg: Option<&String>, data: &[u8]) -> Result<(), Box<dyn Error>> {
  match arg {
    None => io::stdout().write_all(data)?,
    Some(f) => fs::File::open(f)?.write_all(data)?,
  };
  Ok(())
}

fn main() -> Result<(), Box<dyn Error>> {
  let mut mode = None;
  let mut level = None;
  let mut file_args = Vec::with_capacity(2);
  for arg in env::args().skip(1) {
    if arg == "-c" || arg == "--compress" {
      mode = Some(Mode::COMPRESS)
    } else if arg == "-d" || arg == "--decompress" {
      mode = Some(Mode::DECOMPRESS)
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
      return help();
    } else {
      file_args.push(arg)
    }
  }
  if file_args.len() > 2 {
    eprintln!(
      "Too many file arguments, detected {}, only needed 2: {:?}",
      file_args.len(),
      file_args
    );
    return help();
  }
  let mode = match mode {
    None => {
      eprintln!("You must specify one of --compress or --decompress!");
      return help();
    }
    Some(m) => m,
  };
  let level = level.unwrap_or(CompressionLevel::Level0);
  let input = input_file(file_args.get(0))?;

  let output = match mode {
    Mode::COMPRESS => archivelib::do_compress_level(&input, level.compression_level())?,
    Mode::DECOMPRESS => archivelib::do_decompress_level(&input, level.compression_level())?,
  };
  output_file(file_args.get(1), &output)?;
  Ok(())
}
