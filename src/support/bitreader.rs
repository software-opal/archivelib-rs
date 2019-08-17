use std::io::{ErrorKind, Read, Result};

pub trait BitRead {
  fn current_bits(&self) -> u16;
  fn is_eof(&self) -> bool;
  fn read_bits(&mut self, bits_to_load: u8) -> Result<()>;
  fn get_bits(&mut self, bits_to_load: u8) -> Result<u16> {
    let bits: u16 = self.current_bits() >> (16 - bits_to_load);
    self.read_bits(bits_to_load)?;
    Ok(bits)
  }
}
pub struct BitReader<R: Read> {
  inner: R,
  eof: bool,
  bits: u16,
  tmp_bits: u8,
  tmp_bits_size: u8,
}

impl<R: Read> From<R> for BitReader<R> {
  fn from(val: R) -> Self {
    BitReader {
      inner: val,
      eof: false,
      bits: 0,
      tmp_bits: 0,
      tmp_bits_size: 0,
    }
  }
}
impl<R: Read> std::fmt::Debug for BitReader<R> {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    f.debug_struct("BitReader")
      .field("eof", &self.eof)
      .field("bits", &self.bits)
      .field("tmp_bits", &self.tmp_bits)
      .field("tmp_bits_size", &self.tmp_bits_size)
      .finish()
  }
}

impl<R: Read> BitRead for BitReader<R> {
  fn current_bits(&self) -> u16 {
    self.bits
  }
  fn is_eof(&self) -> bool {
    self.eof
  }
  fn read_bits(&mut self, mut bits_to_load: u8) -> Result<()> {
    /*
    Reads `bits_to_load` bits into the LSB side of `data->bits`.
    */
    println!("Read bits: {}, state: {:?}", bits_to_load, self);
    while bits_to_load > self.tmp_bits_size {
      // This loop loads 1 new byte into `data->tmp_bits`(the temporary
      // buffer)
      bits_to_load -= self.tmp_bits_size;
      // Rotate in the remaining bits from the tmp_bit_buffer.
      self.bits = ((self.bits) << self.tmp_bits_size)
        + (u16::from(self.tmp_bits) >> (8 - self.tmp_bits_size));
      let mut tmp = [0];
      self.tmp_bits = match self.inner.read_exact(&mut tmp) {
        Ok(()) => tmp[0],
        Err(err) => {
          if err.kind() == ErrorKind::UnexpectedEof {
            self.eof = true;
            0
          } else {
            return Err(err);
          }
        }
      };
      self.tmp_bits_size = 8;
    }
    self.tmp_bits_size -= bits_to_load;
    self.bits = (self.bits << bits_to_load) + (u16::from(self.tmp_bits) >> (8 - bits_to_load));
    self.tmp_bits = self.tmp_bits.wrapping_shl(u32::from(bits_to_load));
    println!("new state: {:?}", self);
    Ok(())
  }
}

pub struct ExactCallBitReader {
  bits: Option<u16>,
  eof: bool,
  index: usize,
  expected_call_and_results: Vec<(u8, u16)>,
}

impl ExactCallBitReader {
  pub fn new(calls: Vec<(u8, u16)>) -> Self {
    ExactCallBitReader {
      bits: None,
      eof: false,
      index: 0,
      expected_call_and_results: calls,
    }
  }
}

impl BitRead for ExactCallBitReader {
  fn current_bits(&self) -> u16 {
    self.bits.unwrap()
  }
  fn is_eof(&self) -> bool {
    self.eof
  }
  fn read_bits(&mut self, bits_to_load: u8) -> Result<()> {
    assert!(
      self.index < self.expected_call_and_results.len(),
      "Call to read_bits after all calls have been used"
    );
    let (expected_bits_to_load, bits) = self.expected_call_and_results[self.index];
    self.index += 1;
    assert_eq!(
      bits_to_load, expected_bits_to_load,
      "Call #{} requested the incorrect number of bits",
      self.index
    );
    self.bits = Some(bits);
    self.eof = self.expected_call_and_results.is_empty();
    Ok(())
  }
}

#[cfg(test)]
mod tests {
  use super::*;
  #[test]
  fn test_bit_reader_correctness() {
    let input: Vec<u8> = vec![0b1100_1010, 0b0110_0110, 0b0111_1011];
    let mut reader = BitReader::from(&input[..]);
    assert_eq!(0x00_00, reader.current_bits());
    reader.read_bits(10).unwrap();
    assert_eq!(0b000000_11001010_01, reader.current_bits());
    reader.read_bits(7).unwrap();
    assert_eq!(0b1001010_01100110_0, reader.current_bits());
    reader.read_bits(4).unwrap();
    assert_eq!(
      0b010_01100110_01111,
      reader.current_bits(),
      "Current bits: {:#b}",
      reader.current_bits()
    );
    reader.read_bits(15).unwrap();
    assert_eq!(
      0b1011_000000000000,
      reader.current_bits(),
      "Current bits: {:#b}",
      reader.current_bits()
    );
  }
  #[test]
  fn test_bit_reader_real_data() {
    let input: Vec<u8> = vec![0b11001010, 0b01100110, 0b01111011];
    let mut reader = BitReader::from(&input[..]);
    assert_eq!(0x00_00, reader.current_bits());
    reader.read_bits(10).unwrap();
    assert_eq!(0b000000_11001010_01, reader.current_bits());
    reader.read_bits(7).unwrap();
    assert_eq!(0b1001010_01100110_0, reader.current_bits());
    reader.read_bits(4).unwrap();
    assert_eq!(
      0b010_01100110_01111,
      reader.current_bits(),
      "Current bits: {:#b}",
      reader.current_bits()
    );
    reader.read_bits(15).unwrap();
    assert_eq!(
      0b1011_000000000000,
      reader.current_bits(),
      "Current bits: {:#b}",
      reader.current_bits()
    );
  }
}
