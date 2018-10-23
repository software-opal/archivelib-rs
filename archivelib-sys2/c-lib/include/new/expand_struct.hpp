#ifndef NEW__EXPAND_DATA_HPP
#define NEW__EXPAND_DATA_HPP

#include <climits>
#include "stor.h"

typedef uint8_t uint8_t;

typedef struct RExpandData {
  ALStorage *input_store;
  ALStorage *output_store;

  uint8_t *input_buffer;
  uint8_t *dat_arr180;
  uint8_t *dat_arr181;
  uint16_t *dat_arr189;
  uint16_t *dat_arr190;
  uint16_t *dat_arr240;
  uint16_t *dat_arr241;
  uint8_t *dat_arr242;

  size_t input_buffer_len;
  size_t dat_arr180_len;
  size_t dat_arr181_len;
  size_t dat_arr189_len;
  size_t dat_arr190_len;
  size_t dat_arr240_len;
  size_t dat_arr241_len;
  size_t dat_arr242_len;

  uint8_t *dat_arr_cursor247;

  int16_t dat172;
  int16_t dat175;
  int16_t dat176;
  uint16_t dat182;
  int16_t dat243;
  uint16_t dat244;
  uint8_t dat245;
  int16_t dat246;
  ssize_t dat248;
} RExpandData;

#endif
