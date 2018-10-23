
#include "r_compress.hpp"

void RCompress::fn230(int32_t bits_to_load219, uint8_t *_209, uint16_t *_231) {
  int32_t run_start226;
  uint16_t _288[18];
  _288[1] = 0;
  for (run_start226 = 1; run_start226 <= 16; run_start226++)
    _288[run_start226 + 1] = (uint16_t)((_288[run_start226] + data->dat_arr167[run_start226]) << 1);
  for (run_start226 = 0; run_start226 < bits_to_load219; run_start226++)
    _231[run_start226] = _288[_209[run_start226]]++;
}
