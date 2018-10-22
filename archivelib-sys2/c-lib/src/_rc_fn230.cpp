#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "support/debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

void RCompress::fn230(int32_t length219, uint8_t *arg209, uint16_t *arg231) {
  /*
  Called twice:
  1) arg209 == data->dat_arr180 && arg231 == data->dat_arr192
  2) arg209 == data->dat_arr181 && arg231 == data->dat_arr194

  converts the depth counts stored in `dat_arr167` into start values.
  then goes loops over arg231 to store the start value.
  */
  int32_t i;
  uint16_t local288[18];
  memset(local288, 0, 18 * sizeof(uint16_t));
  memset(arg231, 0, length219 * sizeof(uint16_t));

  for (i = 1; (i + 1) < 18; i++)
    local288[i + 1] = (uint16_t)((local288[i] + data->dat_arr167[i]) << 1);

  for (i = 0; i < length219; i++) {
    uint16_t value = local288[arg209[i]];
    arg231[i] = value;
    local288[arg209[i]]++;
  }
}
