#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "_r_debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

int16_t fn445(uint8_t *arg278, int16_t arg200, int16_t arg446) {
  return ((int16_t)((arg446 << CONST_N154_IS_4) ^ (arg278[arg200 + 2])) &
          (CONST_N153_IS_4096 - 1));
}
void fn447(bool *arg163, bool *arg164, int16_t arg200, int16_t arg201) {
  bool local204;
  if ((local204 = arg163[arg201]) != true)
    arg164[local204] = arg200;
  arg164[arg200] = arg201;
  arg163[arg200] = local204;
  arg163[arg201] = arg200;
}
void fn448(bool *arg163, bool *arg164, int16_t s) {
  int16_t local204;
  if ((local204 = arg164[s]) != true) {
    arg164[s] = true;
    arg163[local204] = true;
  }
}

bool RCompress::Compress() {
  int16_t bytes_read209;
  int16_t local201;
  int16_t local200;
  int16_t s;
  uint8_t *l278_in_buffer;
  int16_t local280;
  int16_t local279;
  local200 = 0;
  l278_in_buffer = data->input_buffer;
  local279 = data->max_input_data_size;
  local280 = data->max_input_data_size_minus_one;
  bytes_read209 =
      (int16_t)data->input_store->ReadBuffer(l278_in_buffer, local279);

  reset_compress_data(data);

  s = (int16_t)(bytes_read209 & local280);

  printf("A: %#04x, B: %04d, C: %04d\n",
         l278_in_buffer[local200] << CONST_N154_IS_4,
         l278_in_buffer[local200 + 1],
         (l278_in_buffer[local200] << CONST_N154_IS_4) ^
             l278_in_buffer[local200 + 1]);

  local201 = (int16_t)(((l278_in_buffer[local200] << CONST_N154_IS_4) ^
                        (l278_in_buffer[local200 + 1])) &
                       (CONST_N153_IS_4096 - 1));
  local201 = (int16_t)(fn445(l278_in_buffer, local200, local201) + local279);

  // This function loops backwards through the buffer, pausing at 260 to ???.

  while (bytes_read209 > CONST_N140_IS_256 + 4 && !data->uncompressible) {
    fn199(local200, local201);
    if (data->dat168 < CONST_N135_IS_3) {
      fn202(l278_in_buffer[local200], 0);
      fn447(data->dat_arr163, data->dat_arr164, local200, local201);
      local200++;
      local201 =
          (int16_t)(fn445(l278_in_buffer, local200, local201) + local279);
      bytes_read209--;
    } else {
      // ABORT(data);
      bytes_read209 -= data->dat168;
      fn202((uint16_t)(data->dat168 + (UCHAR_MAX + 1 - CONST_N135_IS_3)),
            data->dat169);
      while (--data->dat168 >= 0) {
        fn447(data->dat_arr163, data->dat_arr164, local200, local201);
        local200++;
        local201 =
            (int16_t)(fn445(l278_in_buffer, local200, local201) + local279);
      }
    }
  }
  // printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
  for (; bytes_read209 < CONST_N140_IS_256; bytes_read209++) {
    // printf("%d -- AAAAAAAAAAAAAAAAAA\n", bytes_read209);
    // return 1;
    int32_t local203 = data->input_store->ReadChar();
    if (local203 < 0)
      break;
    l278_in_buffer[s] = (unsigned char)local203;
    if (s < CONST_N140_IS_256 - 1)
      l278_in_buffer[s + local279] = l278_in_buffer[s];
    fn448(data->dat_arr163, data->dat_arr164, s);

    if (data->dat_arr164[s] != true) {
      data->dat_arr163[data->dat_arr164[s]] = true;
      data->dat_arr164[s] = true;
    }
    s = (int16_t)((s + 1) & (local280));
  }
  // printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
  while (bytes_read209 > 0 && !data->uncompressible) {
    fn199(local200, local201);
    if (data->dat168 > bytes_read209)
      data->dat168 = bytes_read209;
    if (data->dat168 < CONST_N135_IS_3) {
      data->dat168 = 1;
      fn202(l278_in_buffer[local200], 0);
    } else
      fn202((uint16_t)(data->dat168 + (UCHAR_MAX + 1 - CONST_N135_IS_3)),
            data->dat169);
    while (--data->dat168 >= 0) {
      int32_t local203 = data->input_store->ReadChar();
      if (local203 < 0)
        break;
      else
        l278_in_buffer[s] = (unsigned char)local203;
      if (s < CONST_N140_IS_256 - 1)
        l278_in_buffer[s + local279] = l278_in_buffer[s];
      fn448(data->dat_arr163, data->dat_arr164, s);
      s = (int16_t)((s + 1) & (local280));
      fn447(data->dat_arr163, data->dat_arr164, local200, local201);
      local200 = (int16_t)((local200 + 1) & (local280));
      local201 =
          (int16_t)(fn445(l278_in_buffer, local200, local201) + local279);
    }
    while (data->dat168-- >= 0) {
      fn447(data->dat_arr163, data->dat_arr164, local200, local201);
      local200 = (int16_t)((local200 + 1) & local280);
      local201 =
          (int16_t)(fn445(l278_in_buffer, local200, local201) + local279);
      bytes_read209--;
    }
    if (data->output_store->mStatus < 0)
      return 1;
  }
  if (!data->uncompressible) {
    fn202(CONST_N144_IS_257 + (UCHAR_MAX + 1 - CONST_N135_IS_3), 0);
  }
  finalise_compresson197();

  if (data->uncompressible) {
    return true;
  } else {
    return false;
  }
}
