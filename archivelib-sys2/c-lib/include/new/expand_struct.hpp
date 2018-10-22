#ifndef NEW__EXPAND_DATA_HPP
#define NEW__EXPAND_DATA_HPP

#include "_r.h"
#include "stor.h"

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
  uint8_t *dat_arr247;

  int16_t bits_buffer_used172;
  int16_t max_input_data_size;
  int16_t max_input_data_size_minus_one;
  uint16_t bits_buffer182;
  int16_t dat243;
  uint16_t dat244;
  uint8_t dat245;
  int16_t dat246;
  ssize_t dat248;
} RExpandData;

#endif
