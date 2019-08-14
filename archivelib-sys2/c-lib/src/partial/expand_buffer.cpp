#include "new/expand.h"
#include "r_expand.hpp"

void read_bits(RExpandData *data, int32_t bits_to_load219) {
  expand_get_bits(data, bits_to_load219);
}

uint16_t get_bits(RExpandData *data, uint8_t bits_to_load219) {
  return expand_get_bits(data, bits_to_load219);
}
