#include "support/debug.hpp"

std::string get_as_binary(uintmax_t value, uint8_t max_bits) {
  std::string str;
  for (size_t i = 0; i < max_bits; i++) {
    str = (((value) & (1 << i)) ? '1' : '0') + str;
  }
  return str;
}
