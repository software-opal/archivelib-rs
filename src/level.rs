#[derive(PartialEq, PartialOrd, Debug)]
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
      0 => Some(CompressionLevel::Level0),
      1 => Some(CompressionLevel::Level1),
      2 => Some(CompressionLevel::Level2),
      3 => Some(CompressionLevel::Level3),
      4 => Some(CompressionLevel::Level4),
      _ => None,
    }
  }
  pub fn buffer_size(&self) -> usize {
    1 << self.compression_factor()
  }
  pub fn compression_factor(&self) -> u8 {
    10 + self.compression_level()
  }
  pub fn compression_level(&self) -> u8 {
    match self {
      CompressionLevel::Level0 => 0,
      CompressionLevel::Level1 => 1,
      CompressionLevel::Level2 => 2,
      CompressionLevel::Level3 => 3,
      CompressionLevel::Level4 => 4,
    }
  }
}
