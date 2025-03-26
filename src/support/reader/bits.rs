use crate::support::bit_iter::ToBits;

use super::BitwiseRead;

#[derive(Debug)]
pub struct BitBasedBitwiseReader {
  permit_read_beyond_data: bool,
  bits: Vec<bool>,
}

impl BitBasedBitwiseReader {
  pub fn new(bits: &[bool]) -> Self {
    let mut bits = Vec::from(bits);
    bits.reverse();
    Self {
      permit_read_beyond_data: true,
      bits,
    }
  }
  pub fn from_bit_string(string: &str) -> Self {
    Self::new(
      &string
        .chars()
        .filter_map(|c| match c {
          '0' => Some(false),
          '1' => Some(true),
          ' ' | '\n' => None,
          _ => panic!("Invalid character: {:?}", c),
        })
        .collect::<Vec<_>>(),
    )
  }
  pub fn from_hex_string(string: &str) -> Self {
    Self::new(
      &string
        .chars()
        .flat_map(|c| match c {
          '0'..='9' | 'a'..='f' | 'A'..='F' => c
            .to_digit(16)
            .unwrap()
            .to_bits()
            .into_iter()
            .rev()
            .take(4)
            .rev()
            .collect::<Vec<_>>(),
          ' ' | '\n' => vec![],
          _ => panic!("Invalid character: {:?}", c),
        })
        .collect::<Vec<_>>(),
    )
  }
  pub fn prevent_read_beyond_data(self) -> Self {
    Self {
      permit_read_beyond_data: false,
      ..self
    }
  }
  pub fn assert_read_exhausted(&self) {
    assert_eq!(self.bits, []);
  }
}
impl BitwiseRead for BitBasedBitwiseReader {
  fn read_bit_or_eof(&mut self) -> std::io::Result<Option<bool>> {
    assert!(
      self.permit_read_beyond_data || !self.bits.is_empty(),
      "Reading bits beyond the bounds of the bit data"
    );
    Ok(self.bits.pop())
  }
}
