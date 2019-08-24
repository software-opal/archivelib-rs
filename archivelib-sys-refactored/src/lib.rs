#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

#[cfg(test)]
extern crate rand;

#[cfg(test)]
mod test;

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

impl AllocatedMemory2 {
  pub fn to_err(mut self: Self) -> Result<Vec<u8>, Option<String>> {
    let raw = if self.data.is_null() {
      None
    } else {
      let mut data = vec![];
      let slice = unsafe { std::slice::from_raw_parts(self.data, self.length) };
      data.extend(slice);
      Some(data)
    };
    unsafe { clean2(&mut self) };

    match (self.status >= 0, raw) {
      (true, Some(d)) => Ok(d),
      (_, Some(other)) => Err(Some(other.into_iter().map(|b| b as char).collect())),
      (_, None) => Err(None),
    }
  }
}

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
  let mut data = {
    let mut v = vec![];
    v.extend(input.iter());
    v.into_boxed_slice()
  };
  let length = data.len();
  let ptr = data.as_mut_ptr();
  unsafe { compress2(ptr, length, compression_level) }
    .to_err()
    .map(|v| v.into_boxed_slice())
    .map_err(|o| o.unwrap_or_else(|| "".to_string()))
}

pub fn do_decompress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  do_decompress_level(input, AL_GREENLEAF_LEVEL_0)
}

pub fn do_decompress_level(
  input: &[u8],
  compression_level: u8,
) -> Result<Box<[u8]>, std::string::String> {
  let mut data = {
    let mut v = vec![];
    v.extend(input.iter());
    v.into_boxed_slice()
  };
  let length = data.len();
  let ptr = data.as_mut_ptr();
  unsafe { decompress2(ptr, length, compression_level) }
    .to_err()
    .map(|v| v.into_boxed_slice())
    .map_err(|o| o.unwrap_or_else(|| "".to_string()))
}
