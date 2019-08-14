
#include "support/compress.h"

#include "r_compress.hpp"

void fn228(RCompressData *data, int32_t _229) {
  int32_t run_start226, _289;
  uint32_t _458;
  for (run_start226 = 0; run_start226 <= 16; run_start226++)
    data->dat_arr167[run_start226] = 0;
  calculate_pointer_depths(data->dat_arr189, data->dat_arr190, data->dat_arr167,
                           0, data->dat174, _229);
  _458 = 0;
  for (run_start226 = 16; run_start226 > 0; run_start226--)
    _458 += data->dat_arr167[run_start226] << (16 - run_start226);
  while (_458 != (1U << 16)) {
    data->dat_arr167[16]--;
    for (run_start226 = 15; run_start226 > 0; run_start226--) {
      if (data->dat_arr167[run_start226] != 0) {
        data->dat_arr167[run_start226]--;
        data->dat_arr167[run_start226 + 1] =
            (uint16_t)(data->dat_arr167[run_start226 + 1] + 2);
        break;
      }
    }
    _458--;
  }
  for (run_start226 = 16; run_start226 > 0; run_start226--) {
    _289 = data->dat_arr167[run_start226];
    while (--_289 >= 0)
      data->dat_arr_cursor178[*data->dat_arr_cursor188++] =
          (uint8_t)run_start226;
  }
}
