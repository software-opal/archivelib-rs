use crate::{
  consts::{EOF_FLAG, MAX_RUN_LENGTH, MIN_RUN_LENGTH},
  support::bit_utils::bit_size,
};

use super::entry::LzssEntry;

pub const BYTE_RUN_MAX_VALUE: usize = EOF_FLAG;
pub const MAX_BUFFER_BEFORE_FLUSH: usize = 8192 - ((3 * 8) + 6);
pub const RUN_OFFSET_MAX_BIT_LENGTH: usize = 14;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Output {
  ByteEncoded(usize),
  OffsetEncoded(usize),
  Bits(u16, usize),
}
impl Output {
  fn byte_encoded(byte: usize) -> Self {
    if byte > EOF_FLAG {
      panic!("Invalid byte {:#05X}", byte)
    };
    Output::ByteEncoded(byte)
  }
}

pub struct LzssBuffer {
  pub current_byte_length: usize,
  pub data: Vec<LzssEntry>,
}

impl LzssBuffer {
  pub fn new() -> Self {
    Self {
      current_byte_length: 0,
      data: Vec::new(),
    }
  }

  pub fn insert_element(&mut self, entry: LzssEntry) -> usize {
    if let LzssEntry::Run(run_len, _offset) = entry {
      assert!(
        MIN_RUN_LENGTH <= run_len,
        "Run length out of bounds: {:}",
        run_len
      );
      assert!(
        run_len <= MAX_RUN_LENGTH,
        "Run length out of bounds: {:}",
        run_len
      );
    }

    self.current_byte_length += entry.byte_size();
    if self.data.len() % 8 == 0 {
      self.current_byte_length += 1;
    }
    self.data.push(entry);
    self.current_byte_length
  }

  pub fn generate_frequency_tables(
    &self,
  ) -> (
    [u16; BYTE_RUN_MAX_VALUE + 1],
    [u16; RUN_OFFSET_MAX_BIT_LENGTH + 1],
  ) {
    let mut byte_or_run_frequency = [0; BYTE_RUN_MAX_VALUE + 1];
    let mut run_offset_bit_count_frequency = [0; RUN_OFFSET_MAX_BIT_LENGTH + 1];

    for entry in self.data.iter() {
      let byte_or_run_length = entry.lookup_value();
      byte_or_run_frequency[byte_or_run_length] += 1;

      if let Some(offset_bits) = entry.offset_bit_size() {
        run_offset_bit_count_frequency[offset_bits] += 1;
      }
    }

    (byte_or_run_frequency, run_offset_bit_count_frequency)
  }

  pub fn is_full(&self) -> bool {
    self.data.len() % 8 == 0 && self.current_byte_length >= MAX_BUFFER_BEFORE_FLUSH
  }

  pub fn drain_as_output(&mut self) -> impl Iterator<Item = Output> {
    self.current_byte_length = 0;
    self.data.drain(..).flat_map(|entry| match entry {
      LzssEntry::Byte(byte) => vec![Output::byte_encoded(byte as usize)],
      LzssEntry::EoF => vec![Output::byte_encoded(EOF_FLAG), Output::OffsetEncoded(0)],
      LzssEntry::Run(run, 0) => vec![
        Output::byte_encoded(0x100 - MIN_RUN_LENGTH + run),
        Output::OffsetEncoded(0),
      ],
      LzssEntry::Run(run, offset) => {
        let offset_bits = bit_size(offset);

        vec![
          Output::byte_encoded(0x100 - MIN_RUN_LENGTH + run),
          Output::OffsetEncoded(offset_bits),
          Output::Bits(cast!(offset as u16), offset_bits - 1),
        ]
      }
    })
  }
}
