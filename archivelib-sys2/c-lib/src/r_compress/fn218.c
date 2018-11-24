
#include "r_compress.hpp"

void fn218(RCompressData *data, int16_t bits_to_load219, int16_t _220,
           int16_t _221) {
  int16_t run_start226, _289;
  while (bits_to_load219 > 0 && data->dat_arr181[bits_to_load219 - 1] == 0)
    bits_to_load219--;
  write_bits_to_buffer(data, _220, bits_to_load219);
  run_start226 = 0;
  while (run_start226 < bits_to_load219) {
    _289 = data->dat_arr181[run_start226++];
    if (_289 <= 6) {
      write_bits_to_buffer(data, 3, _289);
    } else
      write_bits_to_buffer(data, _289 - 3, (uint16_t)(USHRT_MAX << 1));
    if (run_start226 == _221) {
      while (run_start226 < 6 && data->dat_arr181[run_start226] == 0)
        run_start226++;
      write_bits_to_buffer(data, 2, (uint16_t)(run_start226 - 3));
    }
  }
}
