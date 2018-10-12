#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

impl AllocatedMemory {
  pub fn to_err(mut self: Self) -> Result<Vec<u8>, Option<Vec<u8>>> {
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
      (_, other) => Err(other),
    }
  }
}

#[cfg(test)]
mod tests {
  use super::*;

  #[test]
  fn test_round_trip() {
    let input: &[u8] = b"what if this gets compressed well good!";
    let compressed_data: Box<[u8]> = {
      let mut data = {
        let mut v = vec![];
        v.extend(input.iter());
        v.into_boxed_slice()
      };
      let length = data.len();
      let ptr = data.as_mut_ptr();
      let re = unsafe { compress(ptr, length) }.to_err();
      re.unwrap().into_boxed_slice()
    };
    assert_eq!(
      compressed_data[..],
      vec![
        0, 40, 74, 14, 104, 47, 254, 6, 123, 10, 226, 124, 168, 48, 197, 208, 117, 202, 0, 54, 57,
        193, 61, 140, 137, 65, 206, 33, 213, 249, 239, 197, 34, 195, 77, 80, 123, 182, 227, 240
      ][..]
    );

    let decompressed_data: Box<[u8]> = {
      let mut data = {
        let mut v = vec![];
        v.extend(compressed_data.iter());
        v.into_boxed_slice()
      };
      let length = data.len();
      let ptr = data.as_mut_ptr();
      let re = unsafe { decompress(ptr, length) }.to_err();
      re.unwrap().into_boxed_slice()
    };
    assert_eq!(input[..], decompressed_data[..])
  }
}
