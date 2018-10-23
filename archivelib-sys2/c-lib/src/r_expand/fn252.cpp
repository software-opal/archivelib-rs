
#include <cassert>

#include "support/debug.hpp"

#include "r_expand.hpp"

uint16_t RExpand::get_bits(uint8_t bits_to_load219) {
  uint16_t bits;
  assert(bits_to_load219 <= 16);
  bits = (uint16_t)(data->bits182 >> (2 * CHAR_BIT - bits_to_load219));
  read_bits(bits_to_load219);
  return bits;
}
