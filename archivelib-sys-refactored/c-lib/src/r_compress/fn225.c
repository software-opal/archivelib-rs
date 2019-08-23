
#include "support/compress.h"

#include "r_compress.hpp"

void fn225(RCompressData *data, int32_t run_start226, uint16_t *_187,
           int16_t *_177, int16_t _227) {
  int32_t run_length276, _289;
  _289 = _177[run_start226];
  while ((run_length276 = 2 * run_start226) <= _227) {
    if (run_length276 < _227 &&
        _187[_177[run_length276]] > _187[_177[run_length276 + 1]]) {
      run_length276++;
    }
    if (_187[_289] <= _187[_177[run_length276]]) {
      break;
    }
    _177[run_start226] = _177[run_length276];
    run_start226 = run_length276;
  }
  _177[run_start226] = (uint16_t)_289;
}
