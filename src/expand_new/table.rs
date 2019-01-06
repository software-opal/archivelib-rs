// [0x00, 0x03,
//
// // Bits lookup
// self.dat_arr240
// // consumed bits lookup(dat_arr180[dat_arr240[x]])
// self.dat_arr180
//
// // Run offset lookup
// self.dat_arr241
// // 241 length lookup
// self.dat_arr181
//
// // Binary tree(ish) pair. No test cases. No worries
// self.dat_arr190
// self.dat_arr189
use crate::support::LookAheadBitwiseRead;
use std::io::Result;

#[allow(dead_code)]
struct LookupTables {
  pub bit_lookup: Vec<u16>,
  pub bit_lookup_len: Vec<usize>,
  pub run_offset_lookup: Vec<usize>,
  pub run_offset_lookup_len: Vec<usize>,
}

#[allow(dead_code)]
impl LookupTables {
  pub fn new() -> Self {
    Self {
      bit_lookup: vec![0; 4096],
      bit_lookup_len: vec![0; 511],
      run_offset_lookup: vec![0; 256],
      run_offset_lookup_len: vec![0; 19],
    }
  }
  pub fn generate(&mut self, reader: &mut impl LookAheadBitwiseRead) -> Result<()> {
    self.generate_run_offset_lookup(reader, true)?;
    self.generate_bit_lookup(reader)?;
    self.generate_run_offset_lookup(reader, false)
    // self.fn253(CONST_N145_IS_19 as i16, CONST_N147_IS_5 as i16, 3)?;
    // self.fn255()?;
    // self.fn253(CONST_N142_IS_15 as i16, CONST_N540_IS_5 as i16, -1)?;
  }
  pub fn generate_run_offset_lookup(
    &mut self,
    reader: &mut impl LookAheadBitwiseRead,
    do_pad_length: bool,
  ) -> Result<()> {
    let bits_to_load: usize = reader.consume(5)?;
    if bits_to_load == 0 {
      let offset_const = reader.consume(5)?;
      for e in self.run_offset_lookup.iter_mut() {
        *e = offset_const;
      }
      for e in self.run_offset_lookup_len.iter_mut() {
        *e = 0;
      }
      Ok(())
    } else {
      let mut i = 0;
      while i < bits_to_load {
        let mut bit_length = reader.consume(3)?;
        if bit_length == 7 {
          while reader.consume(1)? {
            bit_length += 1;
          }
        }
        self.run_offset_lookup_len[i] = bit_length;
        i += 1;
        if do_pad_length && i == 3 {
          let pad_length: usize = reader.consume(2)?;
          for _ in 0..pad_length {
            self.run_offset_lookup_len[i] = 0;
            i += 1;
          }
        }
      }
      while i < self.run_offset_lookup_len.len() {
        self.run_offset_lookup_len[i] = 0;
        i += 1;
      }
      Ok(())
    }
  }

  pub fn generate_bit_lookup(&mut self, _reader: &mut impl LookAheadBitwiseRead) -> Result<()> {
    panic!();
  }
}

#[cfg(test)]
mod tests {
  use super::*;
  use crate::support::LookAheadBitwiseReader;

  #[test]
  fn reader_calls() {
    let data: Vec<u8> = vec![0x00, 0x03, 0x20, 0x04, 0x3F, 0xF0, 0x1A, 0xE7, 0xC0, 0x02];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);

    assert_eq!(reader.consume::<u16>(16).unwrap(), 0b0000000000000011);
    assert_eq!(reader.consume::<u16>(5).unwrap(), 0b0000000000000100);
  }

  #[test]
  fn base_data_seperated_calls() {
    let data: Vec<u8> = vec![0x00, 0x03, 0x20, 0x04, 0x3F, 0xF0, 0x1A, 0xE7, 0xC0, 0x02];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);
    reader.consume_bits(16).unwrap();
    let mut tables = LookupTables::new();
    tables
      .generate_run_offset_lookup(&mut reader, true)
      .unwrap();
    assert_eq!(
      tables.run_offset_lookup_len,
      rvec![0x00 => 2, 0x01 => 2, 0x00 => 15]
    );
    assert_eq!(tables.run_offset_lookup, rvec![0x02 => 128, 0x03 => 128]);
  }

  #[test]
  fn base_data() {
    // Uncompressed data is [0x1A, 0x1A]
    let data: Vec<u8> = vec![0x00, 0x03, 0x20, 0x04, 0x3F, 0xF0, 0x1A, 0xE7, 0xC0, 0x02];
    let mut reader = LookAheadBitwiseReader::new(&data[..]);
    reader.consume_bits(16).unwrap();
    let mut tables = LookupTables::new();
    tables.generate(&mut reader).unwrap();

    // The generate functon should have read 9.5 bytes(76 bits)

    assert_eq!(reader.look_ahead_bits(2).unwrap().len(), 0);
    assert_eq!(tables.bit_lookup, rvec![0x1A => 2408, 0x1FE => 2048]);
    assert_eq!(
      tables.bit_lookup_len,
      rvec![0x00 => 29, 0x01 => 1, 0x00 => 483, 0x01 => 1]
    );
    assert_eq!(tables.run_offset_lookup, rvec![0x00 => 256]);
    assert_eq!(tables.run_offset_lookup_len, rvec![0x00 => 19]);
  }

}
