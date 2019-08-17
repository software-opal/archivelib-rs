#include "new/expand.h"
#include "r_expand.hpp"
#include "support/debug.h"

// void read_bits__(RExpandData *data, int32_t bits_to_load219, const char*
// file, unsigned int line) { printf("read_bits: %i %s:%i\n", bits_to_load219,
// file, line);
void read_bits(RExpandData *data, int32_t bits_to_load219) {
  expand_get_bits(data, bits_to_load219);
}

// uint16_t get_bits__(RExpandData *data, uint8_t bits_to_load219, const char*
// file, unsigned int line) { printf("get_bits: %i %s:%i\n", bits_to_load219,
// file, line);
uint16_t get_bits(RExpandData *data, uint8_t bits_to_load219) {
  return expand_get_bits(data, bits_to_load219);
}
