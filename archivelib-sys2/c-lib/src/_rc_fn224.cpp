#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "support/debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

void RCompress::fn224(uint16_t arg204) {
  uint16_t local203, local457;
  local203 = 0;
  local457 = arg204;
  while (local457 != 0) {
    local203++;
    local457 = local457 >> 1;
  }
  write_bits_to_buffer(data->dat_arr181[local203], data->dat_arr194[local203]);
  if (local203 > 1) {
    write_bits_to_buffer(local203 - 1, arg204);
  }
}
