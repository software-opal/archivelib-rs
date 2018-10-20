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

#[cfg(test)]
mod tests {

  use super::*;

  use rand::distributions::{Binomial, Poisson};
  use rand::{thread_rng, Rng};
  use std::iter;

  fn get_data(max_len: usize, average_run_len: usize, show: bool) -> Box<[u8]> {
    let mut rand = thread_rng();
    let run_len_dist = Poisson::new(average_run_len as f64);
    let mut data = Vec::with_capacity(max_len);
    if show {
      print!("Data: [");
    }
    loop {
      let run_len: usize = rand.sample(run_len_dist) as usize;
      if run_len + data.len() >= max_len {
        break;
      } else if run_len == 0 {
        continue;
      }
      let val: u8 = rand.gen();
      if show {
        if data.len() != 0 {
          print!(", ");
        }
        print!(
          "{{\"start\": {:#02X}, \"end\": {:#02X}, \"len\": {}, \"val\": {:#02X}}}",
          data.len(),
          data.len() + run_len,
          run_len,
          val
        );
      }
      data.extend(iter::repeat(val).take(run_len));
    }
    if show {
      println!("]");
    }
    data.into_boxed_slice()
  }

  fn to_series_info(data: &[u8]) -> Vec<(u8, usize)> {
    let mut series_info = vec![];
    if data.len() == 0 {
      return series_info;
    }
    let mut last_val = data[0];
    let mut count = 0;
    for &val in data {
      if val != last_val {
        series_info.push((last_val, count));
        last_val = val;
        count = 0;
      }
      count += 1;
    }
    series_info.push((last_val, count));
    series_info
  }

  fn assert_series_arrays_equal(left: &[u8], right: &[u8]) {
    let mut errors = vec![];
    if left.len() != right.len() {
      errors.push(format!("Lengths differ: {} != {}", left.len(), right.len()));
    }
    let left_series_info = to_series_info(left);
    let right_series_info = to_series_info(right);
    if left_series_info.len() != right_series_info.len() {
      errors.push(format!(
        "Series counts differ: {} != {}",
        left_series_info.len(),
        right_series_info.len()
      ));
    }
    for (i, (&(lval, lcount), (rval, rcount))) in
      left_series_info.iter().zip(right_series_info).enumerate()
    {
      if lval != rval {
        errors.push(format!(
          "Series #{} has wrong values: {} != {}",
          i, lval, rval
        ));
      }
      if lcount != rcount {
        errors.push(format!(
          "Series #{} has wrong counts: {} != {}",
          i, lcount, rcount
        ));
      }
      if errors.len() > 5 {
        break;
      }
    }
    let empty: Vec<String> = Vec::new();
    assert_eq!(empty, errors);
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
  fn test_single_sample() {
    let mut rand = thread_rng();
    println!("\nSample: [");
    let len = 128 * 8;
    let input = get_data(len, rand.gen_range(2, 20), true);
    let compressed_data = do_compress(&input);
    let decompressed_data = do_decompress(&compressed_data);
    assert_series_arrays_equal(&input, &decompressed_data);
    assert_eq!(input[..], decompressed_data[..]);
    println!("]\n");
  }

  #[ignore]
  #[test]
  fn test_small_samples() {
    let mut rand = thread_rng();
    let max_data = 128;
    let length_distribution = Binomial::new(max_data, 16.0 / max_data as f64);
    for _ in 0..200 {
      let len: usize = rand.sample(length_distribution) as usize;
      let input = get_data(len, rand.gen_range(5, 50), false);
      let compressed_data = do_compress(&input);
      let decompressed_data = do_decompress(&compressed_data);
      assert_series_arrays_equal(&input, &decompressed_data);
      assert_eq!(input[..], decompressed_data[..]);
    }
  }

  #[ignore]
  #[test]
  fn test_medium_samples() {
    let mut rand = thread_rng();
    let max_data = (1 << 14) - 1;
    let length_distribution = Binomial::new(max_data, 4096.0 / max_data as f64);
    for _ in 0..40 {
      let len: usize = rand.sample(length_distribution) as usize;
      let input = get_data(len, rand.gen_range(5, 50), false);
      let compressed_data = do_compress(&input);
      let decompressed_data = do_decompress(&compressed_data);
      assert_series_arrays_equal(&input, &decompressed_data);
      assert_eq!(input[..], decompressed_data[..]);
    }
  }

  #[ignore]
  #[test]
  fn test_large_samples() {
    let mut rand = thread_rng();
    let max_data = (1 << 14) - 1;
    let length_distribution = Binomial::new(max_data, (1 << 13) as f64 / max_data as f64);
    for _ in 0..10 {
      let len: usize = rand.sample(length_distribution) as usize;
      let input = get_data(len, rand.gen_range(5, 50), false);
      let compressed_data = do_compress(&input);
      let decompressed_data = do_decompress(&compressed_data);
      assert_series_arrays_equal(&input, &decompressed_data);
      assert_eq!(input[..], decompressed_data[..]);
    }
  }

  #[ignore]
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
