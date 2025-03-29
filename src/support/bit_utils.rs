pub fn truncate_bits(bits: u16, len: usize) -> u16 {
  if len == 0 {
    0
  } else {
    bits & (0xFFFF >> (16 - len))
  }
}

/// Iterates from highest bit to lowest bit
pub fn to_bits(bits: u16, bit_count: usize) -> impl Iterator<Item = bool> {
  (0..bit_count).rev().map(move |i| bits & (1 << i) != 0)
}

pub fn bit_size(mut offset: usize) -> usize {
  let mut bits = 0;
  while offset != 0 {
    bits += 1;
    offset >>= 1;
  }
  bits
}

#[cfg(test)]
mod test {
  use super::*;

  #[test]
  fn test_truncate_bits() {
    assert_eq!(truncate_bits(0xFFFF, 0), 0);
    assert_eq!(truncate_bits(0xFFFF, 4), 0xF);
    assert_eq!(truncate_bits(0xFFFF, 16), 0xFFFF);
  }

  #[test]
  fn test_to_bits() {
    assert_eq!(
      to_bits(0xF1, 6).collect::<Vec<_>>(),
      [true, true, false, false, false, true]
    );
    assert_eq!(to_bits(0x0, 16).collect::<Vec<_>>(), [false; 16]);
    assert_eq!(to_bits(0xFF, 0).collect::<Vec<_>>(), []);
  }

  #[test]
  fn test_bit_size() {
    assert_eq!(bit_size(0), 0);
    assert_eq!(bit_size(0b0110), 3);
    assert_eq!(bit_size(0xFF), 8);
  }
}
