#ifndef NEW__COMPRESS_DATA_HPP
#define NEW__COMPRESS_DATA_HPP

#include "_r.h"
#include "stor.h"

typedef struct RCompressData {
  ALStorage *input_store;
  ALStorage *output_store;

  int16_t *dat_arr163;
  int16_t *dat_arr164;
  uint8_t *dat_arr165;
  uint8_t *input_buffer;
  uint16_t *dat_arr167;
  int16_t *dat_arr177;
  uint8_t *buffer;
  uint8_t *dat_arr180;
  uint8_t *dat_arr181;
  uint16_t *dat_arr189;
  uint16_t *dat_arr190;
  uint16_t *dat_arr191;
  uint16_t *dat_arr192;
  uint16_t *dat_arr193;
  uint16_t *dat_arr194;

  size_t dat_arr163_len;
  size_t dat_arr164_len;
  size_t dat_arr165_len;
  size_t input_buffer_len;
  size_t dat_arr167_len;
  size_t dat_arr177_len;
  size_t buffer_len;
  size_t dat_arr180_len;
  size_t dat_arr181_len;
  size_t dat_arr189_len;
  size_t dat_arr190_len;
  size_t dat_arr191_len;
  size_t dat_arr192_len;
  size_t dat_arr193_len;
  size_t dat_arr194_len;

  uint8_t *dat_arr_cursor178;
  uint16_t *dat_arr_cursor187;
  uint16_t *dat_arr_cursor188;

  size_t chars_written;
  size_t input_length;
  bool uncompressible;
  bool fail_uncompressible;

  int16_t dat168;
  int16_t dat169;
  int16_t buffer_position;
  uint16_t bits_buffer_used172;
  int16_t dat173;
  int16_t dat174;
  int16_t max_input_data_size;
  int16_t max_input_data_size_minus_one;
  uint16_t bits_buffer182;
  uint16_t dat183_IS_CONST_8162;
  uint16_t array165_counter;
  uint16_t bitwise_counter185;
  uint16_t array165_tmp_counter186;
} RCompressData;

#endif
