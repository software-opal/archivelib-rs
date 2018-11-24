
#include "r_compress.hpp"

void fn224(RCompressData *data, uint16_t _204) {
  uint16_t byte_or_run_length203, _457;
  byte_or_run_length203 = 0;
  _457 = _204;
  while (_457) {
    byte_or_run_length203++;
    _457 >>= 1;
  }
  write_bits_to_buffer(data, data->dat_arr181[byte_or_run_length203],
                       data->dat_arr194[byte_or_run_length203]);
  if (byte_or_run_length203 > 1)
    write_bits_to_buffer(data, byte_or_run_length203 - 1, _204);
}
