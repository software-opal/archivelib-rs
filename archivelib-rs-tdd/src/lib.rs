#[macro_use]
extern crate failure_derive;
extern crate failure;

#[cfg(test)]
extern crate rand;

mod consts;
mod expand;
mod lookup_table;
pub mod support;

pub fn do_compress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  unimplemented!();
}

pub use expand::do_decompress;

#[cfg(test)]
mod test;
