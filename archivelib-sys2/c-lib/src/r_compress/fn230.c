
#include "support/compress.h"

#include "r_compress.hpp"

void fn230(RCompressData *data, int32_t bits_to_load219, uint8_t *item209,
           uint16_t *_231) {
  // Sibling method is fn258
  // Called with:
  // (CONST_N141_IS_511, dat_arr180, dat_arr192)
  // (CONST_N145_IS_19, dat_arr181, dat_arr194)
  // (CONST_N142_IS_15, dat_arr181, dat_arr194)
  DEBUG_FILE_HANDLE(fs, data);
  fs << "{\"ptr\": " << (intptr_t)(data);
  WRITE_HEX(fs, "bits_to_load219", bits_to_load219);
  WRITE_DATA_ARRAY(fs, data, dat_arr167, uint16_t);
  WRITE_ARRAY_PTR(fs, data, "item209", item209, uint8_t);
  int32_t run_start226;
  uint16_t lookup_table288[18];
  lookup_table288[0] = 0;
  lookup_table288[1] = 0;
  for (run_start226 = 1; run_start226 <= 16; run_start226++)
    lookup_table288[run_start226 + 1] = (uint16_t)(
        (lookup_table288[run_start226] + data->dat_arr167[run_start226]) << 1);
  WRITE_ARRAY(fs, "lookup_table288_pre", lookup_table288, uint16_t, 18);
  for (run_start226 = 0; run_start226 < bits_to_load219; run_start226++)
    _231[run_start226] = lookup_table288[item209[run_start226]]++;
  WRITE_ARRAY(fs, "lookup_table288_post", lookup_table288, uint16_t, 18);
  WRITE_ARRAY_PTR(fs, data, "_231", _231, uint16_t);
  fs << "}";
}
