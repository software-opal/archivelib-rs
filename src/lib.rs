#[macro_use]
extern crate failure_derive;
use failure;

#[cfg(test)]
#[macro_use]
mod test;
#[cfg(test)]
mod proptests;
#[cfg(test)]
mod tests;

#[macro_use]
pub mod support;
pub mod level;
pub use self::level::CompressionLevel;

pub mod expand_new;

mod compress;
mod consts;

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
  let writer = Vec::with_capacity(1024);
  let mut res = match compress::RCompressData::new_with_io_writer(
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

  Ok(res.into_writer().checked_into_inner().into_boxed_slice())
}

pub fn do_decompress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  do_decompress_level(input, AL_GREENLEAF_LEVEL_0)
}
// pub fn do_decompress_level(
//   input: &[u8],
//   compression_level: u8,
// ) -> Result<Box<[u8]>, std::string::String> {
//   let reader = support::BitReader::from(input);
//   let writer = Vec::with_capacity(1024);
//
//   let mut res = match expand::RExpandData::new(reader, writer, input.len(), compression_level + 10)
//   {
//     Ok(res) => res,
//     Err(err) => return Err(format!("{}", err)),
//   };
//
//   match res.expand() {
//     Ok(()) => {}
//     Err(err) => return Err(format!("{}", err)),
//   };
//
//   Ok(res.into_writer().into_boxed_slice())
// }

pub fn do_decompress_level(
  input: &[u8],
  compression_level: u8,
) -> Result<Box<[u8]>, std::string::String> {
  let mut reader = support::lookahead_reader::LookAheadBitwiseReader::new(input);
  let mut writer = Vec::with_capacity(1024);
  let level = match CompressionLevel::from_compression_level(compression_level) {
    Some(l) => l,
    None => return Err(format!("Invalid compression level {}", compression_level)),
  };

  match expand_new::expand(&mut reader, &mut writer, level) {
    Ok(_) => Ok(writer.into_boxed_slice()),
    Err(err) => Err(format!("{:?}", err)),
  }
}
