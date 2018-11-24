
#include "r_compress.hpp"

void fn222(RCompressData *data) {
  int16_t run_start226, _289, bits_to_load219, _277;
  bits_to_load219 = CONST_N141_IS_511;
  while (bits_to_load219 > 0 && data->dat_arr180[bits_to_load219 - 1] == 0)
    bits_to_load219--;
  write_bits_to_buffer(data, CONST_N143_IS_9, bits_to_load219);
  run_start226 = 0;
  while (run_start226 < bits_to_load219) {
    _289 = data->dat_arr180[run_start226++];
    if (_289 == 0) {
      _277 = 1;
      while (run_start226 < bits_to_load219 &&
             data->dat_arr180[run_start226] == 0) {
        run_start226++;
        _277++;
      }
      if (_277 <= 2) {
        for (_289 = 0; _289 < _277; _289++)
          write_bits_to_buffer(data, data->dat_arr181[0], data->dat_arr194[0]);
      } else if (_277 <= 18) {
        write_bits_to_buffer(data, data->dat_arr181[1], data->dat_arr194[1]);
        write_bits_to_buffer(data, 4, (uint16_t)(_277 - 3));
      } else if (_277 == 19) {
        write_bits_to_buffer(data, data->dat_arr181[0], data->dat_arr194[0]);
        write_bits_to_buffer(data, data->dat_arr181[1], data->dat_arr194[1]);
        write_bits_to_buffer(data, 4, 15);
      } else {
        write_bits_to_buffer(data, data->dat_arr181[2], data->dat_arr194[2]);
        write_bits_to_buffer(data, CONST_N143_IS_9, (uint16_t)(_277 - 20));
      }
    } else
      write_bits_to_buffer(data, data->dat_arr181[_289 + 2],
                           data->dat_arr194[_289 + 2]);
  }
}
