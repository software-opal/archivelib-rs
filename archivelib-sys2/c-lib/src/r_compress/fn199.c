
#include "r_compress.hpp"

void fn199(RCompressData *data, int16_t uncompressed_buffer_index200,
           int16_t _201) {
  uint8_t *_451;
  uint8_t *l_uncompressed_buffer278;
  int16_t run_start226, _452, _204, _453;
  _452 = MAX_COMPRESSION_CYCLES;
  data->dat168 = 0;
  _451 = &data->uncompressed_buffer[uncompressed_buffer_index200];
  _204 = _201;
  printf("Arguments: %d, %d\n",uncompressed_buffer_index200, _201);
  while ((_204 = data->dat_arr163[_204]) != TRUE157) {
    if (--_452 < 0)
      break;
    l_uncompressed_buffer278 = &data->uncompressed_buffer[_204];
    printf(": %d to %d\n", uncompressed_buffer_index200, _204);
    if (_451[data->dat168] != l_uncompressed_buffer278[data->dat168])
      continue;
    if (_451[0] != l_uncompressed_buffer278[0])
      continue;
    if (_451[1] != l_uncompressed_buffer278[1])
      continue;
    if (_451[2] != l_uncompressed_buffer278[2])
      continue;
    for (run_start226 = 3; run_start226 < MAX_RUN_LENGTH140; run_start226++)
      if (_451[run_start226] != l_uncompressed_buffer278[run_start226])
        break;
    printf("%d -- %d\n",_452, run_start226);
    if (run_start226 > data->dat168) {
      _453 = (int16_t)(uncompressed_buffer_index200 - _204 - 1);
      if (_453 < 0)
        _453 += data->max_uncompressed_data_size;
      printf("off: %d; %d:%d\n", _453, uncompressed_buffer_index200, _204);
      if (_453 >= data->max_uncompressed_data_size) {
        break;
      }
      data->dat169 = _453;
      if ((data->dat168 = run_start226) >= MAX_RUN_LENGTH140)
        break;
    }
  }
  // if (data->dat168 != 0) {
    DEBUG_FILE_HANDLE(fs, data);
    fs << "{\"ptr\": " << (intptr_t)(data);
    WRITE_HEX(fs, "max_uncompressed_data_size", data->max_uncompressed_data_size);
    WRITE_HEX(fs, "uncompressed_buffer_index200", uncompressed_buffer_index200);
    WRITE_HEX(fs, "_201", _201);
    WRITE_DATA_ARRAY(fs, data, dat_arr163, int16_t);
    WRITE_DATA_ARRAY(fs, data, uncompressed_buffer, uint8_t);
    if (data->dat168 != 0) {
      WRITE_DEC(fs, "dat168", data->dat168);
      WRITE_DEC(fs, "dat169", data->dat169);
    }
    fs << "}";
    fs.close();
  // }
}
