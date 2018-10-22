#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "_r_debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

void RCompress::fn207() {
  uint32_t i, local289, local229, local454, local455;
  uint32_t local456 = 0;

  local229 = fn211(CONST_N141_IS_511, data->bit_pattern_occurrences191, data->dat_arr180,
                   data->dat_arr192);

  local455 = data->bit_pattern_occurrences191[local229];
  write_bits_to_buffer(16, (uint16_t)local455);
  if (local229 >= CONST_N141_IS_511) {
    uint16_t local217[2 * CONST_N145_IS_19 - 1];
    memset(local217, 0xff, (2 * CONST_N145_IS_19 - 1) * sizeof(uint16_t));
    fn216(local217);
    std::cout << "Calling fn211 with local217\n";
    local229 =
        fn211(CONST_N145_IS_19, local217, data->dat_arr181, data->dat_arr194);
    if (local229 >= CONST_N145_IS_19) {
      fn218(CONST_N145_IS_19, CONST_N147_IS_5, 3);
    } else {
      write_bits_to_buffer(CONST_N147_IS_5, 0);
      write_bits_to_buffer(CONST_N147_IS_5, (uint16_t)local229);
    }
    fn222();
  } else {
    write_bits_to_buffer(CONST_N147_IS_5, 0);
    write_bits_to_buffer(CONST_N147_IS_5, 0);
    write_bits_to_buffer(CONST_N143_IS_9, 0);
    write_bits_to_buffer(CONST_N143_IS_9, (uint16_t)local229);
  }
  std::cout << "Calling fn211 with data->dat_arr193\n";
  local229 = fn211(CONST_N142_IS_15, data->dat_arr193, data->dat_arr181,
                   data->dat_arr194);
  if (local229 >= CONST_N142_IS_15) {
    fn218(CONST_N142_IS_15, CONST_N540_IS_5, -1);
  } else {
    write_bits_to_buffer(CONST_N540_IS_5, 0);
    write_bits_to_buffer(CONST_N540_IS_5, (uint16_t)local229);
  }
  local454 = 0;
  for (i = 0; i < local455; i++) {
    if (i % CHAR_BIT == 0)
      local456 = data->dat_arr165[local454++];
    else
      local456 <<= 1;
    if (local456 & (1U << (CHAR_BIT - 1))) {
      write_stored_bits_to_buffer(
          (int16_t)(data->dat_arr165[local454++] + (1U << CHAR_BIT)));
      local289 = data->dat_arr165[local454++];
      local289 += data->dat_arr165[local454++] << CHAR_BIT;
      fn224((int16_t)local289);
    } else
      write_stored_bits_to_buffer(data->dat_arr165[local454++]);
    if (data->uncompressible)
      return;
  }
  memset(data->bit_pattern_occurrences191, 0, CONST_N141_IS_511 * sizeof(uint16_t));
  memset(data->dat_arr193, 0, CONST_N142_IS_15 * sizeof(uint16_t));
}
