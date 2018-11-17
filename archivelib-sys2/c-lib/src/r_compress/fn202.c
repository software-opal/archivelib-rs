
#include "support/compress.h"

#include "r_compress.hpp"

void fn202 (RCompressData *data, uint16_t byte_or_run_length203, uint16_t _204) {
  DC;
  if ((data->bitwise_counter185 >>= 1) == 0) {
    data->bitwise_counter185 = 1U << (CHAR_BIT - 1);
    if (data->array165_counter >= data->dat183_IS_CONST_8162) {
      fn207(data);
      if (data->uncompressible)
        return;
      data->array165_counter = 0;
    }
    data->array165_tmp_counter186 = data->array165_counter++;
    data->dat_arr165[data->array165_tmp_counter186] = 0;
  }
  data->dat_arr165[data->array165_counter++] = (uint8_t)byte_or_run_length203;
  data->dat_arr191[byte_or_run_length203]++;
  if (byte_or_run_length203 >= (1U << CHAR_BIT)) {
    data->dat_arr165[data->array165_tmp_counter186] |=
        (uint8_t)data->bitwise_counter185;
    data->dat_arr165[data->array165_counter++] = (uint8_t)_204;
    data->dat_arr165[data->array165_counter++] = (uint8_t)(_204 >> CHAR_BIT);
    byte_or_run_length203 = 0;
    while (_204) {
      byte_or_run_length203++;
      _204 >>= 1;
    }
    data->dat_arr193[byte_or_run_length203]++;
  }
  DC;
}
