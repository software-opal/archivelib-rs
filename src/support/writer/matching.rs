use super::base::BitwiseWrite;
use crate::support::writer::base::{to_bits, truncate_bits};

pub struct MatchingBitWriter<'a> {
  expected: &'a [u8],
  byte_offset: usize,
  bit_offset: usize,
}

impl<'a> MatchingBitWriter<'a> {
  pub fn new(input: &'a [u8]) -> Self {
    Self {
      expected: input,
      byte_offset: 0,
      bit_offset: 0,
    }
  }
}

impl<'a> BitwiseWrite for MatchingBitWriter<'a> {
  fn write_bits(&mut self, bits: u16, bit_count: usize) -> std::io::Result<()> {
    if bit_count == 0 {
      return Ok(());
    }

    to_bits(bits, bit_count).enumerate().for_each(|(idx, bit)| {
      if self.byte_offset >= self.expected.len() {
        panic!("Writing beyond the bounds of the expected data of length {}. Mismatch was at bit {} of a {} bit write of {:#04$b}.", self.expected.len(), idx, bit_count, truncate_bits(bits, bit_count), bit_count + 2);
      }
      let expected_bit = self.expected[self.byte_offset] & (1 << self.bit_offset) != 0;
      if bit != expected_bit {
        panic!("Incorrect bit written at byte {}[{}]. The mismatch was at bit {} of a {} bit write of {:#05$b} ", self.byte_offset, self.bit_offset, idx, bit_count, truncate_bits(bits, bit_count), bit_count + 2);
      }
      self.bit_offset = (self.bit_offset + 1)  % 8;
      if self.bit_offset == 0 {
        self.byte_offset += 1;
      }
    });
    Ok(())
  }
  fn finalise(&mut self) -> std::io::Result<()> {
    if self.bit_offset > 0 && self.byte_offset + 1 == self.expected.len() {
      self.write_bits(0, 8 - self.bit_offset).unwrap();
    }
    if self.bit_offset == 0 && self.byte_offset == self.expected.len() {
      Ok(())
    } else if self.byte_offset < self.expected.len() {
      panic!("Incomplete");
    } else {
      panic!("")
    }
  }
}
