#[macro_use]
extern crate failure_derive;
extern crate failure;

#[cfg(test)]
extern crate rand;

mod consts;
mod expand;
pub mod support;

pub fn do_compress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  unimplemented!();
}

pub fn do_decompress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  unimplemented!();
}

#[cfg(test)]
mod test;
