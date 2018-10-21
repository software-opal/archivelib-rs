use rand::distributions::{Binomial};
use rand::{thread_rng, Rng};

use crate::test::util::{assert_series_arrays_equal, get_data};
use crate::{do_compress, do_decompress};

#[ignore]
#[test]
fn test_small_samples() {
  let mut rand = thread_rng();
  let max_data = 128;
  let length_distribution = Binomial::new(max_data, 16.0 / max_data as f64);
  for _ in 0..200 {
    let len: usize = rand.sample(length_distribution) as usize;
    let input = get_data(len, rand.gen_range(5, 50), false);
    let compressed_data = do_compress(&input).unwrap();
    let decompressed_data = do_decompress(&compressed_data).unwrap();
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
    let compressed_data = do_compress(&input).unwrap();
    let decompressed_data = do_decompress(&compressed_data).unwrap();
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
    let compressed_data = do_compress(&input).unwrap();
    let decompressed_data = do_decompress(&compressed_data).unwrap();
    assert_series_arrays_equal(&input, &decompressed_data);
    assert_eq!(input[..], decompressed_data[..]);
  }
}
