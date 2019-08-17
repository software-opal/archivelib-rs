#[macro_use]
mod array_alias;

mod base;
mod buffer;
#[allow(clippy::module_inception)]
mod compress;
mod fn199;
mod fn202;
mod fn207;
mod fn211;
mod fn216;
mod fn218;
mod fn222;
mod fn224;
mod fn225;
mod fn228;
mod fn230;

#[cfg(test)]
mod test;

pub use self::base::{
  CompressError, CompressU16ArrayAlias, CompressU8ArrayAlias, RCompressData, Result,
};
