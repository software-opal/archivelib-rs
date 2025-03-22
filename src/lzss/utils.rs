


pub fn bit_size(mut offset: usize) -> usize {
  let mut bits = 0;
  while offset != 0 {
    bits += 1;
    offset >>= 1;
  }
  bits
}