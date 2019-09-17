
#include "support/compress.h"

#include "r_compress.hpp"

void fn216(RCompressData *data, uint16_t *_217) {
  DC;
  int16_t run_start226, _289, bits_to_load219, _277;
  for (run_start226 = 0; run_start226 < CONST_N145_IS_19; run_start226++) {
    _217[run_start226] = 0;
  }
  bits_to_load219 = CONST_N141_IS_511;
  while (bits_to_load219 > 0 && data->dat_arr180[bits_to_load219 - 1] == 0) {
    bits_to_load219--;
  }
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
        _217[0] += _277;
      } else if (_277 <= 18) {
        _217[1]++;
      } else if (_277 == 19) {
        _217[0]++;
        _217[1]++;
      } else {
        _217[2]++;
      }
    } else {
      _217[_289 + 2]++;
    }
  }
  DC;
}
