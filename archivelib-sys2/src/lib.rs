#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

#[cfg(test)]
extern crate rand;

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

  use rand::distributions::{Binomial, Poisson};
  use rand::{thread_rng, Rng};
  use std::iter;

  fn get_data(max_len: usize, average_run_len: usize) -> Box<[u8]> {
    let mut rand = thread_rng();
    let run_len_dist = Poisson::new(average_run_len as f64);
    let mut data = Vec::with_capacity(max_len);
    print!("Data: [");
    loop {
      let run_len: usize = rand.sample(run_len_dist) as usize;
      if run_len + data.len() >= max_len {
        break;
      } else if run_len == 0 {
        continue;
      }
      let val: u8 = rand.gen();
      print!(
        "{{\"start\": {:#02X}, \"end\": {:#02X}, \"len\": {}, \"val\": {:#02X}}}",
        data.len(),
        data.len() + run_len,
        run_len,
        val
      );
      if data.len() != 0 {
        print!(", ");
      }
      data.extend(iter::repeat(val).take(run_len));
    }
    println!("]");
    data.into_boxed_slice()
  }

  fn do_compress(input: &[u8]) -> Box<[u8]> {
    let mut data = {
      let mut v = vec![];
      v.extend(input.iter());
      v.into_boxed_slice()
    };
    let length = data.len();
    let ptr = data.as_mut_ptr();
    let re = unsafe { compress(ptr, length) }.to_err();
    re.unwrap().into_boxed_slice()
  }
  fn do_decompress(input: &[u8]) -> Box<[u8]> {
    let mut data = {
      let mut v = vec![];
      v.extend(input.iter());
      v.into_boxed_slice()
    };
    let length = data.len();
    let ptr = data.as_mut_ptr();
    let re = unsafe { decompress(ptr, length) }.to_err();
    re.unwrap().into_boxed_slice()
  }

  #[test]
  fn test_samples() {
    let mut rand = thread_rng();
    let max_data = 512;
    let length_distribution = Binomial::new(max_data, 256.0 / max_data as f64);
    for i in 0..2 {
      println!("Starting run {}: [", i);
      let len: usize = rand.sample(length_distribution) as usize;
      let input = get_data(len, rand.gen_range(5, 50));
      let compressed_data = do_compress(&input);
      let decompressed_data = do_decompress(&compressed_data);
      assert_eq!(input[..], decompressed_data[..]);
      println!("]\nFinished run {}\n", i);
    }
  }

  #[test]
  fn test_round_trip() {
    let input: &[u8] = b"what if this gets compressed well good!";
    let compressed_data = do_compress(input);
    assert_eq!(
      compressed_data[..],
      vec![
        0, 40, 74, 14, 104, 47, 254, 6, 123, 10, 226, 124, 168, 48, 197, 208, 117, 202, 0, 54, 57,
        193, 61, 140, 137, 65, 206, 33, 213, 249, 239, 197, 34, 195, 77, 80, 123, 182, 227, 240
      ][..]
    );

    let decompressed_data = do_decompress(&compressed_data);
    assert_eq!(input[..], decompressed_data[..])
  }
}