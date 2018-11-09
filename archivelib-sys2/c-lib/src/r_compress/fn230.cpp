
#include "r_compress.hpp"

void RCompress::fn230(int32_t bits_to_load219, uint8_t *item209, uint16_t *_231) {
  int32_t run_start226;
  uint16_t lookup_table288[18];
  lookup_table288[1] = 0;
  for (run_start226 = 1; run_start226 <= 16; run_start226++)
    lookup_table288[run_start226 + 1] = (uint16_t)((lookup_table288[run_start226] + data->dat_arr167[run_start226]) << 1);
  for (run_start226 = 0; run_start226 < bits_to_load219; run_start226++)
    _231[run_start226] = lookup_table288[item209[run_start226]]++;
}
