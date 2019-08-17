#![deny(clippy::cargo)]
#![deny(clippy::fallible_impl_from)]
#![deny(clippy::restriction::wrong_pub_self_convention)]
#![deny(clippy::style::assertions_on_constants)]
#![deny(clippy::style::wrong_self_convention)]
#![deny(clippy::unseparated_literal_suffix)]
#![warn(clippy::pedantic)]
#![allow(clippy::cargo_common_metadata)]
#![allow(clippy::cognitive_complexity)]
#![allow(clippy::module_name_repetitions)]
#![allow(clippy::needless_pass_by_value)]
#![allow(clippy::non_ascii_literal)]
#![allow(clippy::too_many_arguments)]
#![allow(clippy::too_many_lines)]

#[macro_use]
extern crate failure_derive;
use failure;

#[macro_use]
mod support;

#[cfg(test)]
#[macro_use]
mod test;

mod level;
pub use self::level::CompressionLevel;

#[cfg(not(feature = "new_impl"))]
mod expand;
#[cfg(feature = "new_impl")]
mod expand_new;

mod compress;
mod consts;

pub fn do_compress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  do_compress_level(input, CompressionLevel::Level0)
}

pub fn do_compress_level(
  input: &[u8],
  compression_level: CompressionLevel,
) -> Result<Box<[u8]>, std::string::String> {
  let reader = input;
  let writer = Vec::with_capacity(1024);
  let mut res = match compress::RCompressData::new_with_io_writer(
    reader,
    writer,
    input.len(),
    compression_level.compression_factor(),
    false,
  ) {
    Ok(res) => res,
    Err(err) => return Err(format!("{}", err)),
  };

  match res.compress() {
    Ok(()) => (),
    Err(err) => return Err(format!("{}", err)),
  };

  Ok(res.into_writer().checked_into_inner().into_boxed_slice())
}

pub fn do_decompress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  do_decompress_level(input, CompressionLevel::Level0)
}

#[cfg(not(feature = "new_impl"))]
pub fn do_decompress_level(
  input: &[u8],
  compression_level: CompressionLevel,
) -> Result<Box<[u8]>, std::string::String> {
  let reader = support::BitReader::from(input);
  let writer = Vec::with_capacity(1024);

  let mut res = match expand::RExpandData::new(
    reader,
    writer,
    input.len(),
    compression_level.compression_factor(),
  ) {
    Ok(res) => res,
    Err(err) => return Err(format!("{}", err)),
  };

  match res.expand() {
    Ok(()) => {}
    Err(err) => return Err(format!("{}", err)),
  };

  Ok(res.into_writer().into_boxed_slice())
}

#[cfg(feature = "new_impl")]
pub fn do_decompress_level(
  input: &[u8],
  level: CompressionLevel,
) -> Result<Box<[u8]>, std::string::String> {
  let mut reader = support::CorrectLookAheadBitwiseReader::from_reader(input);
  let mut writer = Vec::with_capacity(1024);
  match expand_new::expand(&mut reader, &mut writer, level) {
    Ok(_) => Ok(writer.into_boxed_slice()),
    Err(err) => Err(format!("{:?}", err)),
  }
}
