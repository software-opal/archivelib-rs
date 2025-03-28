use super::Compressor;
use super::Result;
use crate::CompressionLevel;
use crate::huffman::sorts::ARCHIVE_LIB_SORT_ALGORITHM;
use crate::huffman::sorts::ArchiveLibSortAlgorithm;
use crate::huffman::sorts::MODERN_SORT_ALGORITHM;
use crate::huffman::sorts::ModernSortAlgorithm;
use crate::huffman::sorts::SortAlgorithm;
use crate::support::writer::BitwiseWrite;
use crate::support::writer::BitwiseWriter;
use std::io::Read;
use std::io::Write;

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub struct ArchivelibConfig<S: SortAlgorithm> {
  pub level: CompressionLevel,
  pub max_size: Option<usize>,
  pub sort_algorithm: S,
}

impl Default for ArchivelibConfig<ArchiveLibSortAlgorithm> {
  fn default() -> Self {
    Self {
      level: CompressionLevel::Level0,
      max_size: Some(65536),
      sort_algorithm: ARCHIVE_LIB_SORT_ALGORITHM,
    }
  }
}

impl From<CompressionLevel> for ArchivelibConfig<ArchiveLibSortAlgorithm> {
  fn from(level: CompressionLevel) -> Self {
    Self {
      level,
      ..Self::default()
    }
  }
}

impl<S: SortAlgorithm> ArchivelibConfig<S> {
  #![allow(dead_code)]

  pub fn with_sort_algorithm<NewS: SortAlgorithm>(
    self,
    sort_algorithm: NewS,
  ) -> ArchivelibConfig<NewS> {
    ArchivelibConfig {
      sort_algorithm,
      level: self.level,
      max_size: self.max_size,
    }
  }

  pub fn with_modern_sort_algorithm(self) -> ArchivelibConfig<ModernSortAlgorithm> {
    self.with_sort_algorithm(MODERN_SORT_ALGORITHM)
  }
  pub fn with_archive_lib_sort_algorithm(self) -> ArchivelibConfig<ArchiveLibSortAlgorithm> {
    self.with_sort_algorithm(ARCHIVE_LIB_SORT_ALGORITHM)
  }

  pub fn compressor<R: Read, W: BitwiseWrite>(self, reader: R, writer: W) -> Compressor<R, W, S> {
    Compressor::new(reader, writer, self.level, self.sort_algorithm)
  }
  pub fn compress(self, reader: impl Read, writer: impl Write) -> Result<()> {
    self.compress_bitstream(reader, BitwiseWriter::new(writer))
  }
  pub fn compress_bitstream(self, reader: impl Read, writer: impl BitwiseWrite) -> Result<()> {
    self.compressor(reader, writer).compress()
  }
}
