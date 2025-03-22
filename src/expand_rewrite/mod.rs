
pub fn do_decompress(input: &[u8]) -> Result<Box<[u8]>, std::string::String> {
  ArchivelibConfig::default()
    .decompress(input)
    .map_err(|err| format!("{}", err))
}

pub fn do_decompress_level(
  input: &[u8],
  compression_level: CompressionLevel,
) -> Result<Box<[u8]>, std::string::String> {
  (ArchivelibConfig {
    level: compression_level,
    ..ArchivelibConfig::default()
  })
  .decompress(input)
  .map_err(|err| format!("{}", err))
}

