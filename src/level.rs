#[derive(PartialEq, PartialOrd, Copy, Clone, Debug)]
#[allow(clippy::pub_enum_variant_names)]

pub enum CompressionLevel {
  Level0,
  Level1,
  Level2,
  Level3,
  Level4,
}

impl CompressionLevel {
  pub fn from_compression_level(i: u8) -> Option<Self> {
    match i {
      0 => Some(Self::Level0),
      1 => Some(Self::Level1),
      2 => Some(Self::Level2),
      3 => Some(Self::Level3),
      4 => Some(Self::Level4),
      _ => None,
    }
  }
  pub fn buffer_size(self) -> usize {
    1 << self.compression_factor()
  }
  pub fn compression_factor(self) -> u8 {
    10 + self.compression_level()
  }
  pub fn compression_level(self) -> u8 {
    match self {
      Self::Level0 => 0,
      Self::Level1 => 1,
      Self::Level2 => 2,
      Self::Level3 => 3,
      Self::Level4 => 4,
    }
  }
}
