use rand::{thread_rng, Rng};

use crate::test::util::{assert_series_arrays_equal, get_data};
use crate::{do_compress, do_decompress};

#[test]
fn test_single_sample() {
  let mut rand = thread_rng();
  println!("\nSample: [");
  let len = 300;
  let input = get_data(len, rand.gen_range(2, 20), true);
  let compressed_data = do_compress(&input).unwrap();
  let decompressed_data = do_decompress(&compressed_data).unwrap();
  assert_series_arrays_equal(&input, &decompressed_data);
  assert_eq!(input[..], decompressed_data[..]);
  println!("]\n");
}
