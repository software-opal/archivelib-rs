#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "support/debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

void RCompress::fn199(int16_t arg200, int16_t arg201) {
  // dat_arr163[0..16384] == 0
  // dat_arr163[16384..20480] == 0

  uint8_t *local451;
  uint8_t *l278_in_buffer;
  int16_t i, local452, local204, local453;
  local452 = MAX_COMPRESSION_CYCLES;
  data->dat168 = 0;
  local451 = &data->input_buffer[arg200];
  local204 = arg201;
  while (data->dat_arr163[local204] != true) {
    local204 = data->dat_arr163[local204];
    --local452;
    if (local452 < 0) {
      break;
    }
    l278_in_buffer = &data->input_buffer[local204];
    if (local451[data->dat168] != l278_in_buffer[data->dat168]) {
      continue;
    }
    if (local451[0] != l278_in_buffer[0]) {
      continue;
    }
    if (local451[1] != l278_in_buffer[1]) {
      continue;
    }
    if (local451[2] != l278_in_buffer[2]) {
      continue;
    }
    for (i = 3; i < CONST_N140_IS_256; i++) {
      if (local451[i] != l278_in_buffer[i]) {
        break;
      }
    }
    if (i > data->dat168) {
      local453 = (int16_t)(arg200 - local204 - 1);
      if (local453 < 0) {
        local453 += data->max_input_data_size;
      }
      if (local453 >= data->max_input_data_size) {
        break;
      }
      data->dat169 = local453;
      if ((data->dat168 = i) >= CONST_N140_IS_256)
        break;
    }
  }
}
