
#include "r_compress.hpp"

void RCompress::fn228(int _229) {
  int _226, _289;
  uint32_t _458;
  for (_226 = 0; _226 <= 16; _226++)
    data->dat_arr167[_226] = 0;
  calculate_pointer_depths(data->dat_arr189, data->dat_arr190, data->dat_arr167,
                           0, data->dat174, _229);
  _458 = 0;
  for (_226 = 16; _226 > 0; _226--)
    _458 += data->dat_arr167[_226] << (16 - _226);
  while (_458 != (1U << 16)) {
    data->dat_arr167[16]--;
    for (_226 = 15; _226 > 0; _226--) {
      if (data->dat_arr167[_226] != 0) {
        data->dat_arr167[_226]--;
        data->dat_arr167[_226 + 1] = (uint16_t)(data->dat_arr167[_226 + 1] + 2);
        break;
      }
    }
    _458--;
  }
  for (_226 = 16; _226 > 0; _226--) {
    _289 = data->dat_arr167[_226];
    while (--_289 >= 0)
      data->dat_arr_cursor178[*data->dat_arr_cursor188++] = (uint8_t)_226;
  }
}
