
#include "r_compress.hpp"

void RCompress::fn224(uint16_t _204) {
  uint16_t _203, _457;
  _203 = 0;
  _457 = _204;
  while (_457) {
    _203++;
    _457 >>= 1;
  }
  write_bits_to_buffer(data->dat_arr181[_203], data->dat_arr194[_203]);
  if (_203 > 1)
    write_bits_to_buffer(_203 - 1, _204);
}
