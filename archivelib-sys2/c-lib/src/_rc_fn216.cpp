#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "_r_debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

void RCompress::fn216(uint16_t *arg217) {
  // IDENTICAL IN COMPOSITION TO fn222
  int16_t i, local289, local219, local277;
  memset(arg217, 0, CONST_N145_IS_19 * sizeof(uint16_t));

  local219 = CONST_N141_IS_511;
  WRITE_DATA_ARRAY(std::cout, data, dat_arr180, uint8_t);
  // Find the last non-zero in dat_arr180 before 511.
  while (local219 > 0 && data->dat_arr180[local219 - 1] == 0) {
    local219--;
  }
  std::cout << "\nLargest: " << local219 << '\n';
  i = 0;
  while (i < local219) {
    local289 = data->dat_arr180[i];
    i++;
    std::cout << "i: " << i << "; local289: " << local289 << '\n';
    if (local289 == 0) {
      local277 = 1;
      while (i < local219 && data->dat_arr180[i] == 0) {
        // Calculates the length of the zero runs in `data->dat_arr180`.
        i++;
        local277++;
      }
      std::cout << "   local277: " << local277 << '\n';
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
  WRITE_ARRAY(std::cout, "arg217", arg217, uint8_t, CONST_N145_IS_19);
  std::cout << "\n";
}
