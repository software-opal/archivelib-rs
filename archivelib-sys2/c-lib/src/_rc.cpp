#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "_r_debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

void RCompress::fn199(int16_t arg200, int16_t arg201) {
  uint8_t *local451;
  uint8_t *l278_in_buffer;
  int16_t i, local452, local204, local453;
  local452 = MAX_COMPRESSION_CYCLES;
  data->dat168 = 0;
  local451 = &data->input_buffer[arg200];
  local204 = arg201;
  while (data->dat_arr163[local204] != true) {
    local204 = data->dat_arr163[local204];
    --local452;
    if (local452 < 0)
      break;
    l278_in_buffer = &data->input_buffer[local204];
    if (local451[data->dat168] != l278_in_buffer[data->dat168])
      continue;
    if (local451[0] != l278_in_buffer[0])
      continue;
    if (local451[1] != l278_in_buffer[1])
      continue;
    if (local451[2] != l278_in_buffer[2])
      continue;
    for (i = 3; i < CONST_N140_IS_256; i++)
      if (local451[i] != l278_in_buffer[i])
        break;
    if (i > data->dat168) {
      local453 = (int16_t)(arg200 - local204 - 1);
      if (local453 < 0)
        local453 += data->max_input_data_size;
      if (local453 >= data->max_input_data_size) {
        break;
      }
      data->dat169 = local453;
      if ((data->dat168 = i) >= CONST_N140_IS_256)
        break;
    }
  }
}
void RCompress::fn202(uint16_t bits203, uint16_t arg204) {
  if ((data->dat185 >>= 1) == 0) {
    data->dat185 = 1U << (CHAR_BIT - 1);
    if (data->dat184 >= data->dat183) {
      fn207();
      if (data->uncompressible)
        return;
      data->dat184 = 0;
    }
    data->dat186 = data->dat184++;
    data->dat_arr165[data->dat186] = 0;
  }
  data->dat_arr165[data->dat184++] = (uint8_t)bits203;
  data->dat_arr191[bits203]++;
  if (bits203 >= (1U << CHAR_BIT)) {
    data->dat_arr165[data->dat186] |= (uint8_t)data->dat185;
    data->dat_arr165[data->dat184++] = (uint8_t)arg204;
    data->dat_arr165[data->dat184++] = (uint8_t)(arg204 >> CHAR_BIT);
    bits203 = 0;
    while (arg204) {
      bits203++;
      arg204 >>= 1;
    }
    data->dat_arr193[bits203]++;
  }
}
void RCompress::fn207() {
  uint32_t i, local289, local229, local454, local455;
  uint32_t local456 = 0;
  std::cout << "Calling fn211 with data->dat_arr191\n";
  RCompressData *old_data = clone_compress_data(data);
  local229 =
      fn211(CONST_N141, data->dat_arr191, data->dat_arr180, data->dat_arr192);
  diff_compress_data(old_data, data);
  abort();
  local455 = data->dat_arr191[local229];
  write_bits_to_buffer(16, (uint16_t)local455);
  if (local229 >= CONST_N141) {
    uint16_t local217[2 * CONST_N145 - 1];
    memset(local217, 0xff, (2 * CONST_N145 - 1) * sizeof(uint16_t));
    fn216(local217);
    std::cout << "Calling fn211 with local217\n";
    local229 = fn211(CONST_N145, local217, data->dat_arr181, data->dat_arr194);
    if (local229 >= CONST_N145) {
      fn218(CONST_N145, CONST_N147, 3);
    } else {
      write_bits_to_buffer(CONST_N147, 0);
      write_bits_to_buffer(CONST_N147, (uint16_t)local229);
    }
    fn222();
  } else {
    write_bits_to_buffer(CONST_N147, 0);
    write_bits_to_buffer(CONST_N147, 0);
    write_bits_to_buffer(CONST_N143, 0);
    write_bits_to_buffer(CONST_N143, (uint16_t)local229);
  }
  std::cout << "Calling fn211 with data->dat_arr193\n";
  local229 =
      fn211(CONST_N142, data->dat_arr193, data->dat_arr181, data->dat_arr194);
  if (local229 >= CONST_N142) {
    fn218(CONST_N142, CONST_N540, -1);
  } else {
    write_bits_to_buffer(CONST_N540, 0);
    write_bits_to_buffer(CONST_N540, (uint16_t)local229);
  }
  local454 = 0;
  for (i = 0; i < local455; i++) {
    if (i % CHAR_BIT == 0)
      local456 = data->dat_arr165[local454++];
    else
      local456 <<= 1;
    if (local456 & (1U << (CHAR_BIT - 1))) {
      fn223((int16_t)(data->dat_arr165[local454++] + (1U << CHAR_BIT)));
      local289 = data->dat_arr165[local454++];
      local289 += data->dat_arr165[local454++] << CHAR_BIT;
      fn224((int16_t)local289);
    } else
      fn223(data->dat_arr165[local454++]);
    if (data->uncompressible)
      return;
  }
  memset(data->dat_arr191, 0, CONST_N141 * sizeof(uint16_t));
  memset(data->dat_arr193, 0, CONST_N142 * sizeof(uint16_t));
}
void RCompress::fn218(int16_t length219, int16_t arg220, int16_t arg221) {
  int16_t i, local289;
  while (length219 > 0 && data->dat_arr181[length219 - 1] == 0)
    length219--;
  write_bits_to_buffer(arg220, length219);
  i = 0;
  while (i < length219) {
    local289 = data->dat_arr181[i++];
    if (local289 <= 6) {
      write_bits_to_buffer(3, local289);
    } else
      write_bits_to_buffer(local289 - 3, (uint16_t)(USHRT_MAX << 1));
    if (i == arg221) {
      while (i < 6 && data->dat_arr181[i] == 0)
        i++;
      write_bits_to_buffer(2, (uint16_t)(i - 3));
    }
  }
}
