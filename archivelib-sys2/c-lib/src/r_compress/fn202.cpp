
#include "r_compress.hpp"

void RCompress::fn202(uint16_t _203, uint16_t _204) {
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
  data->dat_arr165[data->array165_counter++] = (uint8_t)_203;
  data->dat_arr191[_203]++;
  if (_203 >= (1U << CHAR_BIT)) {
    data->dat_arr165[data->array165_tmp_counter186] |=
        (uint8_t)data->bitwise_counter185;
    data->dat_arr165[data->array165_counter++] = (uint8_t)_204;
    data->dat_arr165[data->array165_counter++] = (uint8_t)(_204 >> CHAR_BIT);
    _203 = 0;
    while (_204) {
      _203++;
      _204 >>= 1;
    }
    data->dat_arr193[_203]++;
  }
}
