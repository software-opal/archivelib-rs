mod data;
// mod fn253;

use self::data::{DataError, LookupTable};

fn build_fn230_reverse_table(arg260: &[u8]) -> Result<[u16; 18], DataError> {
  let mut temp = [0u16; 17];
  let mut lookup_table = [0u16; 18];
  for &element in arg260 {
    temp[element as usize] += 1;
  }
  for i in 1..17 {
    // CAUTION: This wraps around to 0 such that lookup_table[17] == 0
    lookup_table[i + 1] = lookup_table[i].wrapping_add(temp[i] << (16 - i));
  }
  if lookup_table[17] == 0 {
    Ok(lookup_table)
  } else {
    // Code 1 in original source
    Err(DataError::InvariantFailed(1))
  }
}

#[cfg(test)]
mod tests {
  use super::*;
  use std::iter;

  macro_rules! _compact_item {
    () => {
      iter::empty()
    };
    ( ( $v:expr => $count:expr ) ) => {
      iter::repeat($v).take($count as usize)
    };
    ( $v:expr ) => {
      iter::once($v)
    };
  }
  macro_rules! compact_vec {
    () => {
      []
    };
    ( $( $t:tt ),* $(,)* ) => {{let mut vec = vec![];
      $( vec.extend(_compact_item!($t)); )*
      vec}};
    ( as &[$type:ty], $( $t:tt ),* $(,)* ) => {
      {
        let mut vec: Vec<$type> = vec![];
        $( vec.extend(_compact_item!($t)); )*
        vec
      }
    };
  }

  mod build_fn230_reverse_table {
    use super::*;

    #[test]
    fn small_test1() {
      let input: &[u8] = &[0, 3, 2, 3, 0, 3, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
      let output: [u16; 18] = [0, 0, 0, 32768, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
      assert_eq!(build_fn230_reverse_table(&input), Ok(output));
    }
    #[test]
    fn small_test2() {
      let input: &[u8] = &[4, 5, 5, 0, 0, 0, 7, 7, 2, 2, 3, 5, 6, 3, 4, 0, 0, 0, 0];
      let output: [u16; 18] = [
        0, 0, 0, 32768, 49152, 57344, 63488, 64512, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      ];
      assert_eq!(build_fn230_reverse_table(&input), Ok(output));
    }

    #[test]
    fn large_test1() {
      let input: &[u8] = &compact_vec![as &[u8], (0 => 128), 4, 3,  0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 5, (0 => 114), 5, 5, (0 => 70), 4, (0 => 177), 1, 5];
      let output: [u16; 18] = [
        0, 0, 32768, 32768, 49152, 57344, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      ];
      assert_eq!(build_fn230_reverse_table(&input), Ok(output));
    }

  }
}
