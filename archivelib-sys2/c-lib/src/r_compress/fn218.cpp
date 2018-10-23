
#include "r_compress.hpp"

void RCompress::fn218(int16_t _219, int16_t _220, int16_t _221) {
  int16_t _226, _289;
  while (_219 > 0 && data->dat_arr181[_219 - 1] == 0)
    _219--;
  write_bits_to_buffer(_220, _219);
  _226 = 0;
  while (_226 < _219) {
    _289 = data->dat_arr181[_226++];
    if (_289 <= 6) {
      write_bits_to_buffer(3, _289);
    } else
      write_bits_to_buffer(_289 - 3, (uint16_t)(USHRT_MAX << 1));
    if (_226 == _221) {
      while (_226 < 6 && data->dat_arr181[_226] == 0)
        _226++;
      write_bits_to_buffer(2, (uint16_t)(_226 - 3));
    }
  }
}
