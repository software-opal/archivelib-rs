
#include "r_compress.hpp"

void RCompress::fn224(uint16_t _204) {
  uint16_t byte_or_run_length203, _457;
  byte_or_run_length203 = 0;
  _457 = _204;
  while (_457) {
    byte_or_run_length203++;
    _457 >>= 1;
  }
  write_bits_to_buffer(data->dat_arr181[byte_or_run_length203], data->dat_arr194[byte_or_run_length203]);
  if (byte_or_run_length203 > 1)
    write_bits_to_buffer(byte_or_run_length203 - 1, _204);
}
