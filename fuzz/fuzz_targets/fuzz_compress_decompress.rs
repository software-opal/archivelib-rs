#![cfg_attr(fuzzing, no_main)]

pub use archivelib::support::debug::io::MatchingWriter;
use archivelib::{CompressionLevel, support::BitwiseReader};

#[macro_use]
mod helper;

fuzz_with_main!(verify_compress_decompress);

fn verify_compress_decompress(data: &[u8]) {
  let level = CompressionLevel::Level0;
  let compressed = archivelib::do_compress_level(&data, level).unwrap();

  let mut writer = MatchingWriter::new(&data);

  archivelib::do_decompress_level_bitstream(
    BitwiseReader::new(&compressed[..]),
    &mut writer,
    level,
  )
  .unwrap();
  writer.assert_complete();
}
