#include "new/expand.hpp"
#include "r_expand.hpp"

void RExpand::read_bits(int32_t bits_to_load219) {
  expand_get_bits(data, bits_to_load219);
}

uint16_t RExpand::get_bits(uint8_t bits_to_load219) {
  return expand_get_bits(data, bits_to_load219);
}
