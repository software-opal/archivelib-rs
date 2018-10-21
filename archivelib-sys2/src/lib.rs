#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

#[cfg(test)]
extern crate rand;

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

impl AllocatedMemory {
  pub fn to_err(mut self: Self) -> Result<Vec<u8>, Option<String>> {
    let raw = if self.data.is_null() {
      None
    } else {
      let mut data = vec![];
      let slice = unsafe { std::slice::from_raw_parts(self.data, self.length) };
      data.extend(slice);
      Some(data)
    };
    unsafe { clean(&mut self) };

    match (self.status >= 0, raw) {
      (true, Some(d)) => Ok(d),
      (_, Some(other)) => Err(Some(other.into_iter().map(|b| b as char).collect())),
      (_, None) => Err(None),
    }
  }
}

pub fn do_compress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  let mut data = {
    let mut v = vec![];
    v.extend(input.iter());
    v.into_boxed_slice()
  };
  let length = data.len();
  let ptr = data.as_mut_ptr();
  unsafe { compress(ptr, length) }
    .to_err()
    .map(|v| v.into_boxed_slice())
    .map_err(|o| o.unwrap_or("".to_string()))
}
pub fn do_decompress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  let mut data = {
    let mut v = vec![];
    v.extend(input.iter());
    v.into_boxed_slice()
  };
  let length = data.len();
  let ptr = data.as_mut_ptr();
  unsafe { decompress(ptr, length) }
    .to_err()
    .map(|v| v.into_boxed_slice())
    .map_err(|o| o.unwrap_or("".to_string()))
}

#[cfg(test)]
mod test;
