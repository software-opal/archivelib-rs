pub const MAX_COMPRESSION_FACTOR: u8 = 14;
pub const MIN_COMPRESSION_FACTOR: u8 = 10;

/// Obfuscated name: _135
pub const MIN_RUN_LENGTH: usize = 3;
/// Obfuscated name: _140
pub const MAX_RUN_LENGTH: usize = 256;
pub const END_OF_FILE_FLAG: usize = MAX_RUN_LENGTH + 1;

/// Defines the limit of the byte/run length/EOF data, `511` or `0x1FF`.
///
/// The data `0..256` represents bytes, `256..510` represents run lengths, `510` represents EOF.
///
/// Obfuscated name: `_141`
pub const CONST_N141_IS_511: usize =
  (u8::MAX as usize) + (MAX_RUN_LENGTH - MIN_RUN_LENGTH) + 3;
/// During compression, this is used as the size of the byte/run table's bit length huffman table.
///
/// Used when calling `_218`.
///
/// Obfuscated name: `_145`
pub const CONST_N145_IS_19: usize = u16::BITS as usize + 3;
/// Same as `_145`
///
/// Obfuscated name: `_152`
pub const CONST_N152_IS_19: usize = CONST_N145_IS_19;

#[cfg(not(feature = "new_impl"))]
/// Obfuscated name: `_148`
pub const CONST_N148_IS_4096: usize = 4096;
#[cfg(not(feature = "new_impl"))]
/// Obfuscated name: `_149`
pub const CONST_N149_IS_256: usize = 256;

/// Obfuscated name: `_153`
pub const BYTE_RUN_HASH_SIZE: usize = 4096;
pub const BYTE_RUN_HASH_BITMASK: u16 = (BYTE_RUN_HASH_SIZE as u16) - 1;
/// Obfuscated name: `_142`
pub const CONST_N142_IS_15: usize = (MAX_COMPRESSION_FACTOR as usize) + 1;
/// Obfuscated name: `_143`
pub const CONST_N143_IS_9: usize = 9;
/// The number of bits to use when writing out the bit length table sizes.
///
/// Likely derived from the number of bits in `19`(the size of the bit size table)
///
/// Obfuscated name: `_147`
pub const CONST_N147_IS_5: usize = 5;
///
///
/// Obfuscated name: `_154`
pub const CONST_N154_IS_4: usize = 4;
/// Obfuscated name: `_155`
pub const CONST_N155_IS_8192: usize = 8192;
/// Obfuscated name: `_540`
pub const CONST_N540_IS_5: usize = 5;

pub const BUFFER_SIZE: usize = 512;
pub const BUFFER_BIT_SIZE: usize = 8 * BUFFER_SIZE;
pub const EOF_ERROR_LIMIT: usize = 5;
