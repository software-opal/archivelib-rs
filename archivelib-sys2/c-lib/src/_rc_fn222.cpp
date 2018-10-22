#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "support/debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

void RCompress::fn222() {
  // IDENTICAL IN COMPOSITION TO fn216
  int16_t i, local289, length219, local277;
  length219 = CONST_N141_IS_511;
  while (length219 > 0 && data->dat_arr180[length219 - 1] == 0)
    length219--;
  write_bits_to_buffer(CONST_N143_IS_9, length219);
  i = 0;
  while (i < length219) {
    local289 = data->dat_arr180[i++];
    if (local289 == 0) {
      local277 = 1;
      while (i < length219 && data->dat_arr180[i] == 0) {
        i++;
        local277++;
      }
      if (local277 <= 2) {
        for (local289 = 0; local289 < local277; local289++)
          write_bits_to_buffer(data->dat_arr181[0], data->dat_arr194[0]);
      } else if (local277 <= 18) {
        write_bits_to_buffer(data->dat_arr181[1], data->dat_arr194[1]);
        write_bits_to_buffer(4, (uint16_t)(local277 - 3));
      } else if (local277 == 19) {
        write_bits_to_buffer(data->dat_arr181[0], data->dat_arr194[0]);
        write_bits_to_buffer(data->dat_arr181[1], data->dat_arr194[1]);
        write_bits_to_buffer(4, 15);
      } else {
        write_bits_to_buffer(data->dat_arr181[2], data->dat_arr194[2]);
        write_bits_to_buffer(CONST_N143_IS_9, (uint16_t)(local277 - 20));
      }
    } else
      write_bits_to_buffer(data->dat_arr181[local289 + 2],
                           data->dat_arr194[local289 + 2]);
  }
}
