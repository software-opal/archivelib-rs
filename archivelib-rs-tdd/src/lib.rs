#![feature(try_from)]
#![feature(const_int_ops)]

#[macro_use]
extern crate failure_derive;
extern crate failure;

#[cfg(test)]
extern crate archivelib_sys;
#[cfg(test)]
#[macro_use]
extern crate proptest;
#[cfg(test)]
extern crate rand;

#[macro_use]
pub mod support;

mod compress;
mod consts;
mod expand;
// #[cfg(test)]
// mod proptests;
#[cfg(test)]
mod test;

pub const AL_GREENLEAF_LEVEL_0: u8 = 0;
pub const AL_GREENLEAF_LEVEL_1: u8 = 1;
pub const AL_GREENLEAF_LEVEL_2: u8 = 2;
pub const AL_GREENLEAF_LEVEL_3: u8 = 3;
pub const AL_GREENLEAF_LEVEL_4: u8 = 4;

pub fn do_compress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  do_compress_level(input, AL_GREENLEAF_LEVEL_0)
}

pub fn do_compress_level(
  input: &[u8],
  compression_level: u8,
) -> Result<Box<[u8]>, std::string::String> {
  let reader = input;
  let writer = support::BitwiseWriter::new(Vec::with_capacity(1024));
  let mut res = match compress::RCompressData::new(
    reader,
    writer,
    input.len(),
    compression_level + 10,
    false,
  ) {
    Ok(res) => res,
    Err(err) => return Err(format!("{}", err)),
  };

  match res.compress() {
    Ok(()) => (),
    Err(err) => return Err(format!("{}", err)),
  };

  return Ok(res.into_writer().into_inner().into_boxed_slice());
}

pub fn do_decompress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  do_decompress_level(input, AL_GREENLEAF_LEVEL_0)
}
pub fn do_decompress_level(
  input: &[u8],
  compression_level: u8,
) -> Result<Box<[u8]>, std::string::String> {
  let reader = support::BitReader::from(input);
  let writer = support::BitwiseWriter::new(Vec::with_capacity(1024));

  let mut res = match expand::RExpandData::new(reader, writer, input.len(), compression_level + 10)
  {
    Ok(res) => res,
    Err(err) => return Err(format!("{}", err)),
  };

  match res.expand() {
    Ok(()) => {}
    Err(err) => return Err(format!("{}", err)),
  };

  return Ok(res.into_writer().into_inner().into_boxed_slice());
}
