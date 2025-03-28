use super::base::BitwiseWrite;
use crate::support::bit_utils::{to_bits, truncate_bits};

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
  pub fn assert_complete(&self) {
    assert_eq!(
      (self.byte_offset, self.bit_offset),
      (self.expected.len(), 0),
      "Write did not complete correctly"
    );
  }
}

impl BitwiseWrite for MatchingBitWriter<'_> {
  fn write_bits(&mut self, bits: u16, bit_count: usize) -> std::io::Result<()> {
    if bit_count == 0 {
      return Ok(());
    }

    to_bits(bits, bit_count).enumerate().for_each(|(idx, bit)| {
      if self.byte_offset >= self.expected.len() {
        panic!("Writing beyond the bounds of the expected data of length {}. Mismatch was at bit {} of a {} bit write of {:#04$b}.", self.expected.len(), idx, bit_count, truncate_bits(bits, bit_count), bit_count + 2);
      }
      let expected_bit = self.expected[self.byte_offset] & (1 << (7 - self.bit_offset)) != 0;
      if bit != expected_bit {
        panic!("Incorrect bit written at byte {}[{}]. The mismatch was at bit {} of a {} bit write of {:#05$b}.", self.byte_offset, self.bit_offset, idx, bit_count, truncate_bits(bits, bit_count), bit_count + 2);
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
      panic!(
        "Incomplete data written, was expecting {} bytes, but only received {}[{}] bytes[bits].",
        self.expected.len(),
        self.byte_offset,
        self.bit_offset
      );
    } else {
      panic!("")
    }
  }
}

#[cfg(test)]
mod test {

  use super::*;

  #[test]
  fn test_write_bits() {
    let input = [0b0101_0101];
    let mut writer = MatchingBitWriter::new(&input);
    writer.write_bits(0b0, 1).unwrap();
    writer.write_bits(0b1, 1).unwrap();
    writer.write_bits(0b01, 2).unwrap();
    writer.write_bits(0b0101, 4).unwrap();
    writer.finalise().unwrap();
  }

  #[test]
  #[should_panic(
    expected = "Incorrect bit written at byte 0[4]. The mismatch was at bit 2 of a 6 bit write of 0b110000."
  )]
  fn test_incorrect_bit_write() {
    let input = [0b1111_1111];
    let mut writer = MatchingBitWriter::new(&input);
    writer.write_bits(0b11, 2).unwrap();
    writer.write_bits(0b1111_0000, 6).unwrap();
  }
  #[test]
  #[should_panic(
    expected = "Writing beyond the bounds of the expected data of length 0. Mismatch was at bit 0 of a 4 bit write of 0b1111."
  )]
  fn test_write_beyond_input_bounds() {
    let input = [];
    let mut writer = MatchingBitWriter::new(&input);
    writer.write_bits(0xFF, 4).unwrap();
  }

  #[test]
  fn test_finalize() {
    let input = [0xFF, 0xF0];
    let mut writer = MatchingBitWriter::new(&input);
    writer.write_bits(0xFF_F, 12).unwrap();
    writer.finalise().unwrap();
  }
  #[test]
  #[should_panic(
    expected = "Incomplete data written, was expecting 3 bytes, but only received 1[4] bytes[bits]."
  )]
  fn test_finalize_too_short() {
    let input = [0xFF, 0xF0, 0x00];
    let mut writer = MatchingBitWriter::new(&input);
    writer.write_bits(0xF_FF, 12).unwrap();
    writer.finalise().unwrap();
  }
  #[test]
  #[should_panic(
    expected = "Incomplete data written, was expecting 2 bytes, but only received 1[0] bytes[bits]."
  )]
  fn test_finalize_on_byte_end() {
    let input = [0xFF, 0x00];
    let mut writer = MatchingBitWriter::new(&input);
    writer.write_bits(0xFF, 8).unwrap();
    writer.finalise().unwrap();
  }
  #[test]
  #[should_panic(
    expected = "Incorrect bit written at byte 1[4]. The mismatch was at bit 0 of a 4 bit write of 0b0000."
  )]
  fn test_finalize_with_non_zero_expected_bits() {
    let input = [0xFF, 0xFF];
    let mut writer = MatchingBitWriter::new(&input);
    writer.write_bits(0xFF_F, 12).unwrap();
    writer.finalise().unwrap();
  }
}
