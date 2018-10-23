
#include "r_compress.hpp"

void RCompress::fn230(int32_t bits_to_load219, uint8_t *_209, uint16_t *_231) {
  int32_t _226;
  uint16_t _288[18];
  _288[1] = 0;
  for (_226 = 1; _226 <= 16; _226++)
    _288[_226 + 1] = (uint16_t)((_288[_226] + data->dat_arr167[_226]) << 1);
  for (_226 = 0; _226 < bits_to_load219; _226++)
    _231[_226] = _288[_209[_226]]++;
}
