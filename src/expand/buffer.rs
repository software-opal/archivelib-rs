use crate::expand::{RExpandData, Result};
use crate::support::BitRead;
use std::io::Write;

impl<R: BitRead, W: Write> RExpandData<R, W> {
  pub fn read_bits(&mut self, bits_to_load219: i16) -> Result<()> {
    self.input_store.read_bits(bits_to_load219 as u8)?;
    self.bits182 = self.input_store.current_bits();
    if self.input_store.is_eof() {
      self.error_counter243 += 1;
    }
    println!(
      "BITS: {:#b}({:#X}) / {}",
      self.bits182, self.bits182, bits_to_load219
    );
    Ok(())
  }

  pub fn get_bits(&mut self, bits_to_load219: i16) -> Result<u16> {
    if bits_to_load219 == 0 {
      return Ok(0);
    }
    let bits: u16 = self.bits182 >> (2 * 8 - bits_to_load219);
    self.read_bits(bits_to_load219)?;
    Ok(bits)
  }
}

#[cfg(test)]
mod tests {
  use super::*;
  use crate::support::ExactCallBitReader;

  // const data: Vec<u8> =

  #[test]
  fn test_reads_data_the_same_as_original() {
    let data = vec![
      0x00, 0x12, 0x43, 0x88, 0x81, 0xA7, 0xFF, 0x0D, 0x9A, 0xC8, 0xF4, 0x61, 0xB4, 0x81, 0x94,
      0x00, 0x20, 0x9B, 0xD4, 0x90, 0x00, 0x00, 0x19, 0x3C, 0x00, 0x62, 0xA5, 0xC1, 0x81, 0xAF,
      0xF0,
    ];
    let length = data.len();
    let out = vec![];
    let expected_call_results = ExactCallBitReader::new(vec![
      (16, 0x0012),
      (16, 0x4388),
      (5, 0x7110),
      (3, 0x8881),
      (3, 0x440d),
      (3, 0x2069),
      (2, 0x81a7),
      (3, 0x0d3f),
      (3, 0x69ff),
      (3, 0x4ffe),
      (3, 0x7ff0),
      (3, 0xff86),
      (9, 0x0d9a),
      (2, 0x366b),
      (9, 0xd647),
      (3, 0xb23d),
      (3, 0x91e8),
      (3, 0x8f46),
      (3, 0x7a30),
      (2, 0xe8c3),
      (4, 0x8c36),
      (4, 0xc369),
      (3, 0x1b48),
      (2, 0x6d20),
      (9, 0x40ca),
      (2, 0x0328),
      (2, 0x0ca0),
      (9, 0x4002),
      (2, 0x0008),
      (2, 0x0020),
      (9, 0x4137),
      (2, 0x04de),
      (2, 0x137a),
      (9, 0xf524),
      (4, 0x5240),
      (2, 0x4900),
      (5, 0x2000),
      (3, 0x0000),
      (3, 0x0000),
      (3, 0x0000),
      (3, 0x0003),
      (3, 0x0019),
      (3, 0x00c9),
      (3, 0x064f),
      (3, 0x3278),
      (3, 0x93c0),
      (3, 0x9e00),
      (3, 0xf001),
      (5, 0x0031),
      (1, 0x0062),
      (1, 0x00c5),
      (1, 0x018a),
      (1, 0x0315),
      (1, 0x062a),
      (1, 0x0c54),
      (1, 0x18a9),
      (1, 0x3152),
      (1, 0x62a5),
      (1, 0xc54b),
      (4, 0x54b8),
      (1, 0xa970),
      (4, 0x9706),
      (3, 0xb830),
      (4, 0x8303),
      (1, 0x0606),
      (7, 0x035f),
      (1, 0x06bf),
      (1, 0x0d7f),
      (1, 0x1aff),
      (1, 0x35fe),
      (1, 0x6bfc),
      (1, 0xd7f8),
      (4, 0x7f80),
      (1, 0xff00),
      (5, 0xe000),
      (4, 0x0000),
    ]);
    let mut test = RExpandData::new(expected_call_results, out, length, 10).unwrap();

    test.expand().unwrap();
  }
}
