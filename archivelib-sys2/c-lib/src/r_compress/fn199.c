
#include "r_compress.hpp"

void fn199 (RCompressData *data, int16_t uncompressed_buffer_index200, int16_t _201) {
  uint8_t *_451;
  uint8_t *l_uncompressed_buffer278;
  int16_t run_start226, _452, _204, _453;
  _452 = MAX_COMPRESSION_CYCLES;
  data->dat168 = 0;
  _451 = &data->uncompressed_buffer[uncompressed_buffer_index200];
  _204 = _201;
  while ((_204 = data->dat_arr163[_204]) != TRUE157) {
    if (--_452 < 0)
      break;
    l_uncompressed_buffer278 = &data->uncompressed_buffer[_204];
    if (_451[data->dat168] != l_uncompressed_buffer278[data->dat168])
      continue;
    if (_451[0] != l_uncompressed_buffer278[0])
      continue;
    if (_451[1] != l_uncompressed_buffer278[1])
      continue;
    if (_451[2] != l_uncompressed_buffer278[2])
      continue;
    for (run_start226 = 3; run_start226 < CONST_N140_IS_256; run_start226++)
      if (_451[run_start226] != l_uncompressed_buffer278[run_start226])
        break;
    if (run_start226 > data->dat168) {
      _453 = (int16_t)(uncompressed_buffer_index200 - _204 - 1);
      if (_453 < 0)
        _453 += data->max_uncompressed_data_size;
      if (_453 >= data->max_uncompressed_data_size) {
        break;
      }
      data->dat169 = _453;
      if ((data->dat168 = run_start226) >= CONST_N140_IS_256)
        break;
    }
  }
}
