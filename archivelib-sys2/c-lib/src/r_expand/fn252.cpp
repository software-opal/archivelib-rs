#include "r_expand.hpp"

uint16_t RExpand::fn252(int32_t bits_to_load219) {
  uint16_t _284;
  _284 = (uint16_t)(data->bits182 >> (2 * CHAR_BIT - bits_to_load219));
  read_bits(bits_to_load219);
  return _284;
}
