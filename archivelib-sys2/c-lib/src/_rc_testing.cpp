#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "_r_debug.hpp"
#include <string>
#include <iostream>
#include <sstream>


void RCompress::fn216(uint16_t *arg217) {
  int16_t i, local289, local219, local277;
  memset(arg217, 0, CONST_N145 * sizeof(uint16_t));

  local219 = CONST_N141;
  while (local219 > 0 && data->dat_arr180[local219 - 1] == 0)
    {local219--;}
  i = 0;
  while (i < local219) {
    local289 = data->dat_arr180[i];
    i++;
    if (local289 == 0) {
      local277 = 1;
      while (i < local219 && data->dat_arr180[i] == 0) {
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

void RCompress::fn222() {
  RCompressData *old_data = clone_compress_data(data);
  int16_t i, local289, local219, local277;
  local219 = CONST_N141;
  while (local219 > 0 && data->dat_arr180[local219 - 1] == 0)
    local219--;
  write_bits_to_buffer(CONST_N143, local219);
  i = 0;
  while (i < local219) {
    local289 = data->dat_arr180[i++];
    if (local289 == 0) {
      local277 = 1;
      while (i < local219 && data->dat_arr180[i] == 0) {
        i++;
        local277++;
      }
      if (local277 <= 2) {
        for (local289 = 0; local289 < local277; local289++)
          write_bits_to_buffer(data->dat_arr181[0], data->dat_arr194[0]);
      } else if (local277 <= 18) {
        write_bits_to_buffer(data->dat_arr181[1], data->dat_arr194[1]);
        write_bits_to_buffer(4, (uint16_t)(local277 - 3));
      } else if (local277 == 19) {
        write_bits_to_buffer(data->dat_arr181[0], data->dat_arr194[0]);
        write_bits_to_buffer(data->dat_arr181[1], data->dat_arr194[1]);
        write_bits_to_buffer(4, 15);
      } else {
        write_bits_to_buffer(data->dat_arr181[2], data->dat_arr194[2]);
        write_bits_to_buffer(CONST_N143, (uint16_t)(local277 - 20));
      }
    } else
      write_bits_to_buffer(data->dat_arr181[local289 + 2],
                           data->dat_arr194[local289 + 2]);
  }
  diff_compress_data(old_data, data);
}
