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

#[cfg(not(feature = "new_impl"))]
mod expand;
#[cfg(feature = "new_impl")]
mod expand_new;

mod compress;
mod config;
mod consts;
mod errors;

pub use self::config::ArchivelibConfig;
pub use self::errors::*;
pub use self::level::CompressionLevel;

#[cfg(feature = "sys")]
pub mod sys {
  pub use archivelib_sys::{do_compress, do_compress_level, do_decompress, do_decompress_level};
}

pub fn do_compress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  ArchivelibConfig::default()
    .compress(input)
    .map_err(|err| format!("{}", err))
}

pub fn do_compress_level(
  input: &[u8],
  compression_level: CompressionLevel,
) -> Result<Box<[u8]>, std::string::String> {
  (ArchivelibConfig {
    level: compression_level,
    ..ArchivelibConfig::default()
  })
  .compress(input)
  .map_err(|err| format!("{}", err))
}

pub fn do_decompress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  ArchivelibConfig::default()
    .decompress(input)
    .map_err(|err| format!("{}", err))
}

pub fn do_decompress_level(
  input: &[u8],
  compression_level: CompressionLevel,
) -> Result<Box<[u8]>, std::string::String> {
  (ArchivelibConfig {
    level: compression_level,
    ..ArchivelibConfig::default()
  })
  .decompress(input)
  .map_err(|err| format!("{}", err))
}
