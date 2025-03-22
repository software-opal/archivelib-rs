mod byte_run_hash_table;
mod huffman_writer;
mod input_ring_buffer;
mod lzss;
mod reader;

use std::{io::Read, io::Write};

use crate::{
  CompressionLevel,
  compress::Result,
  huffman::sorts::{ARCHIVE_LIB_SORT_ALGORITHM, ArchiveLibSortAlgorithm, SortAlgorithm},
  support::BitwiseWriter,
};

pub use self::reader::Compressor;

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

  pub fn compress(self, reader: impl Read, writer: impl Write) -> Result<()> {
    Compressor::new(
      reader,
      BitwiseWriter::new(writer),
      self.level.compression_factor(),
      self.sort_algorithm,
    )
    .and_then(|mut compressor| compressor.compress())
  }
}
