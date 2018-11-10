#[macro_use]
extern crate failure_derive;
extern crate failure;

#[cfg(test)]
extern crate rand;

#[cfg(test)]
mod test;

mod consts;
mod expand;
mod support;

pub fn do_compress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  unimplemented!();
}

pub use expand::do_decompress;
