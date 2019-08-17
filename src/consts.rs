pub const MAX_COMPRESSION_FACTOR: u8 = 14;
pub const MIN_COMPRESSION_FACTOR: u8 = 10;

pub const MIN_RUN_LENGTH135_IS_3: usize = 3;
pub const MAX_RUN_LENGTH140: usize = 256;
pub const END_OF_FILE_FLAG: usize = MAX_RUN_LENGTH140 + 1;

pub const CONST_N141_IS_511: usize =
  (u8::max_value() as usize) + (MAX_RUN_LENGTH140 - MIN_RUN_LENGTH135_IS_3) + 3;
pub const CONST_N145_IS_19: usize = (u16::max_value().count_ones() as usize + 3);
pub const CONST_N152_IS_19: usize = CONST_N145_IS_19;

pub const CONST_N153_IS_4096: usize = 4096;
pub const CONST_N153_SUB_1_IS_4095: u16 = 4095; // CONST_N153_IS_4096 - 1
pub const CONST_N142_IS_15: usize = (MAX_COMPRESSION_FACTOR as usize) + 1;
pub const CONST_N143_IS_9: usize = 9;
pub const CONST_N147_IS_5: usize = 5;
pub const CONST_N154_IS_4: usize = 4;
pub const CONST_N155_IS_8192: usize = 8192;
pub const CONST_N540_IS_5: usize = 5;
// #define TRUE157 (-1)
