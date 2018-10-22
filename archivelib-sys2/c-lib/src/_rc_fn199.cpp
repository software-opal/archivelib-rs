#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "_r_debug.hpp"
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
  local204 = arg201;
  // WRITE_ARRAY(std::cout, "dat_arr163[arg201...",
  // (&data->dat_arr163[local204]),
  //             bool, 20);
  while (!data->dat_arr163[local204]) {
    local204 = data->dat_arr163[local204] ? 1 : 0;
    --local452;
    if (local452 < 0) {
      break;
    }
    // WRITE_ARRAY(std::cout, "input_buffer[arg200...",
    //             (&data->input_buffer[arg200]), bool, 20);
    // WRITE_ARRAY(std::cout, "input_buffer[local204...",
    //             (&data->input_buffer[local204]), bool, 20);
    if (data->input_buffer[arg200 + data->dat168] !=
        data->input_buffer[local204 + data->dat168]) {
      continue;
    }
    if (data->input_buffer[arg200] != data->input_buffer[local204]) {
      continue;
    }
    if (data->input_buffer[arg200 + 1] != data->input_buffer[local204 + 1]) {
      continue;
    }
    if (data->input_buffer[arg200 + 2] != data->input_buffer[local204 + 2]) {
      continue;
    }
    for (i = 3; i < CONST_N140_IS_256; i++) {
      if (data->input_buffer[arg200 + i] != data->input_buffer[local204 + i]) {
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
      data->dat168 = i;
      if (i >= CONST_N140_IS_256) {
        break;
      }
    }
  }
}
