#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "support/debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

void RCompress::fn228(int32_t arg229) {
  size_t i, j, cursor_idx = 0;
  uint32_t local458;
  memset(data->dat_arr167, 0, 17 * sizeof(uint16_t));

  calculate_pointer_depths(data->dat_arr189, data->dat_arr190, data->dat_arr167,
                           0, data->dat174, arg229);

  local458 = 0;
  for (i = 1; i < 17; i++) {
    local458 += data->dat_arr167[i] << (16 - i);
  }
  if (local458 != 0x10000) {
    // This appears to be an incredibly rare event.
    std::cout << "IS THIS A CASE?";
    WRITE_DATA_ARRAY(std::cout, data, dat_arr167, uint16_t);
    std::cout << "\n";
    COMPRESS_ABORT(data);
    while (local458 != (1U << 16)) {
      data->dat_arr167[16]--;
      for (i = 15; i > 0; i--) {
        if (data->dat_arr167[i] != 0) {
          data->dat_arr167[i]--;
          data->dat_arr167[i + 1] = (uint16_t)(data->dat_arr167[i + 1] + 2);
          break;
        }
      }
      local458--;
    }
  }

  /*
    Called twice per compression.
    1) dat_arr_cursor178 == dat_arr180 && dat_arr_cursor188 == dat_arr192
    2) dat_arr_cursor178 == dat_arr181 && dat_arr_cursor188 == dat_arr194

    The cursor appears to be unused beyond this method so refactored to use
    array indexing instead of pointer math.

    This outer loop needs to go backwards so elements of 188 are accessed in the
    correct order.
  */
  for (i = 16; i > 0; i--) {
    for (j = 0; j < data->dat_arr167[i]; j++, cursor_idx++) {
      data->dat_arr_cursor178[data->dat_arr_cursor188[cursor_idx]] = (uint8_t)i;
    }
  }
}
