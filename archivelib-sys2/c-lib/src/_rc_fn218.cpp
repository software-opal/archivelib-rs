#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "_r_debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

void RCompress::fn218(int16_t length219, int16_t arg220, int16_t arg221) {
  printf("fn218\n");
  int16_t i, local289;
  while (length219 > 0 && data->dat_arr181[length219 - 1] == 0)
    length219--;
  write_bits_to_buffer(arg220, length219);
  i = 0;
  while (i < length219) {
    local289 = data->dat_arr181[i++];
    if (local289 <= 6) {
      write_bits_to_buffer(3, local289);
    } else
      write_bits_to_buffer(local289 - 3, (uint16_t)(USHRT_MAX << 1));
    if (i == arg221) {
      while (i < 6 && data->dat_arr181[i] == 0)
        i++;
      write_bits_to_buffer(2, (uint16_t)(i - 3));
    }
  }
}
