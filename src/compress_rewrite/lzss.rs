use super::input_ring_buffer::{MAX_RUN_LENGTH, MIN_RUN_LENGTH};

pub const EOF_FLAG: usize = 0x1FE;
pub const BYTE_RUN_MAX_VALUE: usize = EOF_FLAG;

pub const RUN_OFFSET_MAX_BIT_LENGTH: usize = 14;
pub const MAX_BYTE_LENGTH: usize = 8192 - ((3 * 8) + 6);

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

fn bit_size(mut offset: usize) -> usize {
  let mut bits = 0;
  while offset != 0 {
    bits += 1;
    offset >>= 1;
  }
  bits
}

#[derive(Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum LzssEntry {
  Byte(u8),
  /// (run length, offset)
  /// Offset starts at `0` indicating the run starts at the byte prior.
  Run(usize, usize),
  EoF,
}
impl LzssEntry {
  fn byte_size(&self) -> usize {
    match self {
      Self::Byte(_) => 1,
      Self::Run(_, _) => 3,
      Self::EoF => 3,
    }
  }
  fn lookup_value(&self) -> usize {
    match self {
      Self::Byte(b) => (*b).into(),
      Self::Run(run, _) => 0x100 + usize::from(*run) - 3,
      Self::EoF => EOF_FLAG,
    }
  }
  fn offset_value(&self) -> Option<usize> {
    match self {
      Self::Byte(_) => None,
      Self::Run(_, offset) => Some((*offset).into()),
      Self::EoF => Some(0),
    }
  }
  fn offset_bit_size(&self) -> Option<usize> {
    self.offset_value().map(bit_size)
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
    return self.data.len() % 8 == 0 && self.current_byte_length >= MAX_BYTE_LENGTH;
  }

  pub fn drain_as_output(&mut self) -> impl Iterator<Item = Output> {
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
