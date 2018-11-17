#ifndef NEW__EXPAND_DATA_HPP
#define NEW__EXPAND_DATA_HPP

#include "new/cpp_compat.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RExpandData {
  ALStorage *input_store;
  ALStorage *output_store;

  uint8_t *uncompressed_buffer;
  uint8_t *dat_arr180;
  uint8_t *dat_arr181;
  uint16_t *dat_arr189;
  uint16_t *dat_arr190;
  uint16_t *dat_arr240;
  uint16_t *dat_arr241;
  uint8_t *compressed_data_buffer242;

  size_t uncompressed_buffer_len;
  size_t dat_arr180_len;
  size_t dat_arr181_len;
  size_t dat_arr189_len;
  size_t dat_arr190_len;
  size_t dat_arr240_len;
  size_t dat_arr241_len;
  size_t compressed_data_buffer242_len;

  size_t compressed_data_index;
  int16_t bits_in_buffer172;
  int16_t max_uncompressed_data_size;
  int16_t max_uncompressed_data_size_bitmask;
  uint16_t bits182;
  int16_t error_counter243;
  uint16_t items_until_next_header;
  uint8_t tmp_bit_buffer245;
  int16_t loaded_compressed_data_length246;
  ssize_t compressed_data_length248;
} RExpandData;

#ifdef __cplusplus
}
#endif

#endif
