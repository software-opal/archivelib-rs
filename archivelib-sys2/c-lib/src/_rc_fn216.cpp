#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "support/debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

void RCompress::fn216(uint16_t *arg217) {
  // IDENTICAL IN COMPOSITION TO fn222
  int16_t i, local289, length219, local277;
  memset(arg217, 0, CONST_N145_IS_19 * sizeof(uint16_t));

  length219 = CONST_N141_IS_511;
  // Find the last non-zero in dat_arr180 before 511.
  while (length219 > 0 && data->dat_arr180[length219 - 1] == 0) {
    length219--;
  }
  i = 0;
  while (i < length219) {
    local289 = data->dat_arr180[i];
    i++;
    if (local289 == 0) {
      local277 = 1;
      while (i < length219 && data->dat_arr180[i] == 0) {
        // Calculates the length of the zero runs in `data->dat_arr180`.
        i++;
        local277++;
      }
      if (local277 <= 2) {
        arg217[0] += local277;
      } else if (local277 <= 18) {
        arg217[1]++;
      } else if (local277 == 19) {
        arg217[0]++;
        arg217[1]++;
      } else {
        arg217[2]++;
      }
    } else {
      arg217[local289 + 2]++;
    }
  }
}
