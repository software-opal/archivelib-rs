
#include "r_compress.hpp"

void RCompress::fn222() {
  int16_t _226, _289, _219, _277;
  _219 = CONST_N141_IS_511;
  while (_219 > 0 && data->dat_arr180[_219 - 1] == 0)
    _219--;
  write_bits_to_buffer(CONST_N143_IS_9, _219);
  _226 = 0;
  while (_226 < _219) {
    _289 = data->dat_arr180[_226++];
    if (_289 == 0) {
      _277 = 1;
      while (_226 < _219 && data->dat_arr180[_226] == 0) {
        _226++;
        _277++;
      }
      if (_277 <= 2) {
        for (_289 = 0; _289 < _277; _289++)
          write_bits_to_buffer(data->dat_arr181[0], data->dat_arr194[0]);
      } else if (_277 <= 18) {
        write_bits_to_buffer(data->dat_arr181[1], data->dat_arr194[1]);
        write_bits_to_buffer(4, (uint16_t)(_277 - 3));
      } else if (_277 == 19) {
        write_bits_to_buffer(data->dat_arr181[0], data->dat_arr194[0]);
        write_bits_to_buffer(data->dat_arr181[1], data->dat_arr194[1]);
        write_bits_to_buffer(4, 15);
      } else {
        write_bits_to_buffer(data->dat_arr181[2], data->dat_arr194[2]);
        write_bits_to_buffer(CONST_N143_IS_9, (uint16_t)(_277 - 20));
      }
    } else
      write_bits_to_buffer(data->dat_arr181[_289 + 2],
                           data->dat_arr194[_289 + 2]);
  }
}
