pub const MAX_COMPRESSION_FACTOR: u8 = 14;
pub const MIN_COMPRESSION_FACTOR: u8 = 10;

/// Obfuscated name: _135
pub const MIN_RUN_LENGTH: usize = 3;
/// Obfuscated name: _140
pub const MAX_RUN_LENGTH: usize = 256;
pub const END_OF_FILE_FLAG: usize = MAX_RUN_LENGTH + 1;

pub const CONST_N141_IS_511: usize =
  (u8::max_value() as usize) + (MAX_RUN_LENGTH - MIN_RUN_LENGTH) + 3;
pub const CONST_N145_IS_19: usize = u16::max_value().count_ones() as usize + 3;
pub const CONST_N152_IS_19: usize = CONST_N145_IS_19;

#[cfg(not(feature = "new_impl"))]
pub const CONST_N148_IS_4096: usize = 4096;
#[cfg(not(feature = "new_impl"))]
pub const CONST_N149_IS_256: usize = 256;

pub const BYTE_RUN_HASH_SIZE: usize = 4096;
pub const BYTE_RUN_HASH_BITMASK: u16 = (BYTE_RUN_HASH_SIZE as u16) - 1;
pub const CONST_N142_IS_15: usize = (MAX_COMPRESSION_FACTOR as usize) + 1;
pub const CONST_N143_IS_9: usize = 9;
pub const CONST_N147_IS_5: usize = 5;
pub const CONST_N154_IS_4: usize = 4;
pub const CONST_N155_IS_8192: usize = 8192;
pub const CONST_N540_IS_5: usize = 5;

pub const BUFFER_SIZE: usize = 512;
pub const BUFFER_BIT_SIZE: usize = 8 * BUFFER_SIZE;
pub const EOF_ERROR_LIMIT: usize = 5;
