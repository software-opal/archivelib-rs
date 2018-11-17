#ifndef NEW__CONST_HPP
#define NEW__CONST_HPP

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

const int32_t UINT16_BIT = 16;
const int32_t MIN_RUN_LENGTH135_IS_3 = 3;
const int32_t MAX_COMPRESSION_FACTOR = 14;
const int32_t MIN_COMPRESSION_FACTOR = 10;
const int32_t CONST_N140_IS_256 = 256;
const uint32_t CONST_N141_IS_511 =
    (UCHAR_MAX + 1 + CONST_N140_IS_256 - MIN_RUN_LENGTH135_IS_3 + 1 + 1);
const uint32_t CONST_N142_IS_15 = (MAX_COMPRESSION_FACTOR + 1);
const int32_t CONST_N143_IS_9 = 9;
const int32_t END_OF_FILE_FLAG = (CONST_N140_IS_256 + 1);
const uint32_t CONST_N145_IS_19 = (UINT16_BIT + 3);
const int32_t CONST_N540_IS_5 = 5;
const int32_t CONST_N147_IS_5 = 5;
const int32_t CONST_N148_IS_4096 = 4096;
const int32_t CONST_N149_IS_256 = 256;
const int32_t CONST_N152_IS_19 = CONST_N145_IS_19;
const int32_t CONST_N153_IS_4096 = 4096;
const int32_t CONST_N154_IS_4 = 4;
const int32_t CONST_N155_IS_8192 = 8192;
// const int32_t BUFFER_SIZE = 512;
const int32_t TRUE157 = (-1);
const int32_t MAX_COMPRESSION_CYCLES = 128;
const int32_t BUFFER_SIZE = 512;

#define INVALID_COMPRESSION_LEVEL_MSG                                          \
  "Incorrect compression level parameter passed to compressor.  Compression "  \
  "level = %d"
#define MEMORY_ALLOCATION_FAILURE_MSG                                          \
  "Memory allocation failure in compression startup"
#define INTERNAL_ERROR_1_MSG                                                   \
  "Internal 1 error in Greenleaf Decompression routine"
#define INTERNAL_ERROR_2_MSG                                                   \
  "Internal 2 error in Greenleaf Decompression routine"

#ifdef __cplusplus
}
#endif

#endif
