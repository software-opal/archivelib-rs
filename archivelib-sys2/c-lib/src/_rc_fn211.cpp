#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "support/debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

int32_t RCompress::fn211(int32_t arg212, uint16_t *arg213, uint8_t *arg214,
                         uint16_t *arg215) {
  // 511, data->bit_pattern_occurrences191, data->dat_arr180, data->dat_arr192
  // arg212 is the length of arg214 and arg215.
  // arg213 is at least `arg212` long

  int32_t i, local276, local289, local292;
  int16_t local227;
  data->dat174 = (int16_t)arg212;
  data->dat_arr_cursor187 = arg213;
  data->dat_arr_cursor178 = arg214;
  local292 = data->dat174;
  local227 = 0;
  data->dat_arr177[1] = 0;
  for (i = 0; i < data->dat174; i++) {
    arg214[i] = 0;
    if (arg213[i]) {
      data->dat_arr177[++local227] = (int16_t)i;
    }
  }

  if (local227 < 2) {
    arg215[data->dat_arr177[1]] = 0;
    return data->dat_arr177[1];
  }
  for (i = local227 / 2; i >= 1; i--)
    fn225(i, data->dat_arr_cursor187, data->dat_arr177, local227);
  data->dat_arr_cursor188 = arg215;
  do {
    i = data->dat_arr177[1];
    if (i < data->dat174)
      *data->dat_arr_cursor188++ = (uint16_t)i;
    data->dat_arr177[1] = data->dat_arr177[local227--];
    fn225(1, data->dat_arr_cursor187, data->dat_arr177, local227);
    local276 = data->dat_arr177[1];
    if (local276 < data->dat174)
      *data->dat_arr_cursor188++ = (uint16_t)local276;
    local289 = local292++;
    data->dat_arr_cursor187[local289] = (uint16_t)(
        data->dat_arr_cursor187[i] + data->dat_arr_cursor187[local276]);
    data->dat_arr177[1] = (int16_t)local289;
    fn225(1, data->dat_arr_cursor187, data->dat_arr177, local227);
    data->dat_arr189[local289] = (uint16_t)i;
    data->dat_arr190[local289] = (uint16_t)local276;
  } while (local227 > 1);
  data->dat_arr_cursor188 = arg215;
  fn228(local289);
  fn230(arg212, arg214, arg215);
  return local289;
}
