#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "support/debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

void RCompress::fn202(uint16_t bits203, uint16_t arg204) {
  if ((data->bitwise_counter185 >>= 1) == 0) {
    data->bitwise_counter185 = 1U << (CHAR_BIT - 1);
    if (data->array165_counter >= data->dat183_IS_CONST_8162) {
      fn207();
      if (data->uncompressible)
        return;
      data->array165_counter = 0;
    }
    data->array165_tmp_counter186 = data->array165_counter++;
    data->dat_arr165[data->array165_tmp_counter186] = 0;
  }
  data->dat_arr165[data->array165_counter++] = (uint8_t)bits203;
  data->bit_pattern_occurrences191[bits203]++;
  if (bits203 >= (1U << CHAR_BIT)) {
    data->dat_arr165[data->array165_tmp_counter186] |=
        (uint8_t)data->bitwise_counter185;
    data->dat_arr165[data->array165_counter++] = (uint8_t)arg204;
    data->dat_arr165[data->array165_counter++] = (uint8_t)(arg204 >> CHAR_BIT);
    bits203 = 0;
    while (arg204) {
      bits203++;
      arg204 >>= 1;
    }
    data->dat_arr193[bits203]++;
  }
}
