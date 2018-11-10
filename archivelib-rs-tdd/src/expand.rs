use crate::consts::*;
use crate::support::{ReadError, Reader, Writer};
use std::io;

pub fn do_decompress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  match ExpandData::new(
    Reader::new(input),     // input_store
    Writer::new(),          // output_store
    input.len(),            // input_length
    MIN_COMPRESSION_FACTOR, // compression_level
  ) {
    Some(mut data) => match data.decompress() {
      Ok(()) => Ok(data.into_output_data()),
      Err(e) => Err(format!("{:?}", e)),
    },
    None => panic!(),
  }
}

macro_rules! blocked_set {
  ($vec:ident = [ $( $start:expr ; $end:expr => $value: expr ),* $(,)* ] ) => {
    $({
        let value = $value;
        let start = $start;
        let end = $end;
        assert!(end <= $vec.len(), "Cannot set range({}:{}) outside bounds of {} (len={})", start, end, stringify!($vec), $vec.len());
        for i in start..end {
          $vec[i] = value;
        }
    })*
  };
}

#[derive(Debug)]
pub struct ExpandData<R> {

  large_lookup: Box<[u16]>, // len: 4096, was: dat_arr240

  input_store: Reader<R>,
  input_length: usize,
  output_store: Writer,
  //
  // uint8_t *uncompressed_buffer;
  // uint8_t *dat_arr180;
  // uint8_t *dat_arr181;
  // uint16_t *dat_arr189;
  // uint16_t *dat_arr190;
  // uint16_t *dat_arr240;
  // uint16_t *dat_arr241;
  // uint8_t *compressed_data_buffer242;
  //
  // size_t compressed_data_index;
  // int16_t bits_in_buffer172;
  // int16_t max_uncompressed_data_size;
  // int16_t max_uncompressed_data_size_bitmask;
  // uint16_t bits182;
  // int16_t error_counter243;
  // uint16_t items_until_next_header;
  // uint8_t tmp_bit_buffer245;
  // int16_t loaded_compressed_data_length246;
  // ssize_t compressed_data_length248;
}

impl<R> ExpandData<R>
where
  R: io::Read + Sized,
{
  pub fn new(
    input_store: Reader<R>,
    output_store: Writer,
    input_length: usize,
    compression_level: u8,
  ) -> Option<Self> {
    if compression_level > MAX_COMPRESSION_FACTOR || compression_level < MIN_COMPRESSION_FACTOR {
      return None;
    }
    Some(ExpandData {
      input_store: input_store,
      input_length: input_length,
      output_store: output_store,
      large_lookup: Box::new([0u16; 4096]),
    })
  }

  pub fn into_output_data(self) -> Box<[u8]> {
    self.output_store.into_data()
  }

  pub fn decompress(&mut self) -> Result<(), ReadError> {
    self.seed_data_tables()?;

    return Ok(());
  }

  pub fn seed_data_tables(&mut self) -> Result<(), ReadError> {
    let _some_val = self.input_store.read_bits(16)?;

    let bits_to_load = self.input_store.read_bits(5)?;
    if bits_to_load == 0 {
      unimplemented!("Left shark");
    } else {
      let idx = 0;
      while (idx < bits_to_load) {
        let run_length = self.input_store.read_bits(3)?;
        if run_length == 0x7 {
          // is this 12 or 13?
          for _ in 0..12 {
            if self.input_store.read_bit()? {
              run_length += 1;
            } else {
              break;
            }
          }
        }

        unimplemented!();
      }
    }
  }
}

#[cfg(test)]
mod test {
  use super::*;

  #[test]
  fn test_create_new_expand_data_succeds() {
    let data: &[u8] = &vec![];
    let res = ExpandData::new(
      Reader::new(data),      // input_store
      Writer::new(),          // output_store
      0,                      // input_length
      MIN_COMPRESSION_FACTOR, // compression_level
    )
    .unwrap();
    assert_eq!(res.input_length, 0);
    assert_eq!(res.into_output_data()[..], []);
  }

  #[test]
  fn test_seed_data_using_1_00_byte_output() {
    let data: &[u8] = &vec![0x00, 0x02, 0x20, 0x04, 0x3F, 0xFB, 0xD3, 0x00, 0x10];
    let mut res = ExpandData::new(
      Reader::new(data),      // input_store
      Writer::new(),          // output_store
      0,                      // input_length
      MIN_COMPRESSION_FACTOR, // compression_level
    )
    .unwrap();
    res.seed_data_tables().unwrap();
    let mut expected_large_lookup = vec![0u16; 4096];
    blocked_set!{
        expected_large_lookup = [
            0; 2048 => 0,
            2048; 4096 => 510,
        ]
    }
    assert_eq!(res.large_lookup, expected_large_lookup.into_boxed_slice());
  }

  #[test]
  fn test_seed_data_using_1_01_byte_output() {
    let data: &[u8] = &vec![0x00, 0x02, 0x22, 0x08, 0x3F, 0xF9, 0xFA, 0x00, 0x02];
    let mut res = ExpandData::new(
      Reader::new(data),      // input_store
      Writer::new(),          // output_store
      0,                      // input_length
      MIN_COMPRESSION_FACTOR, // compression_level
    )
    .unwrap();
    res.seed_data_tables().unwrap();
    let mut expected_large_lookup = vec![0u16; 4096];
    blocked_set!{
        expected_large_lookup = [
            0; 2048 => 1,
            2048; 4096 => 510,
        ]
    }
    assert_eq!(res.large_lookup, expected_large_lookup.into_boxed_slice());
  }

}
