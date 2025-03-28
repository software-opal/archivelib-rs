#![cfg_attr(fuzzing, no_main)]

use archivelib::{
  CompressionLevel,
  support::{BitwiseWrite, writer::MatchingBitWriter},
};

#[macro_use]
mod helper;

fuzz_with_main!(compress_correctness);

fn compress_correctness(data: &[u8]) {
  let level = CompressionLevel::Level0;
  let compressed_sample =
    archivelib_sys::do_compress_level(&data, level.compression_level()).unwrap();

  let mut writer = MatchingBitWriter::new(&compressed_sample);
  // Matching bit writer panics when an invalid bit is found.
  archivelib::do_compress_level_bitstream(data, &mut writer, level).unwrap();

  writer.assert_complete();
}
