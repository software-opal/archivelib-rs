use std::iter;

use rand::distributions::Poisson;
use rand::{thread_rng, Rng};

pub fn get_data(max_len: usize, average_run_len: usize, show: bool) -> Box<[u8]> {
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
    println!(",{}]", data.len());
  }
  data.into_boxed_slice()
}

pub fn to_series_info(data: &[u8]) -> Vec<(u8, usize)> {
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

pub fn assert_series_arrays_equal(left: &[u8], right: &[u8]) {
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
