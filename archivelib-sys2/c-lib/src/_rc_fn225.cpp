#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "support/debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

void RCompress::fn225(int32_t i, uint16_t *arg187, int16_t *arg177,
                      int16_t arg227) {
  /*
    arg187 == data->dat_arr_cursor187, arg177 == data->dat_arr177

    arg187 can be `bit_pattern_occurrences191` or a local variable
    arg177 is a array of index pointers to arg187.

    This is some sort of rotation function in arg177.
  */
  // std::cout << "\n";
  // std::cout << "fn225: i=" << i << "; arg227=" << arg227 << "\n";
  // WRITE_ARRAY_PTR(std::cout, data, "arg177", arg177, int16_t);
  // std::cout << "\n";

  int32_t local276, local289;
  local289 = arg177[i];
  while ((local276 = 2 * i) <= arg227) {
    // std::cout << "  Considering " << local276 << "\n";
    if (local276 < arg227) {
      // std::cout << "   Inside bounds of " << arg227 << "\n";
      // std::cout << "   Lookup arg187[arg177[local276]] = arg187[" <<
      // arg177[local276] << "] = " << arg187[arg177[local276]] << "\n";
      // std::cout << "   Lookup arg187[arg177[local276+1]] = arg187[" <<
      // arg177[local276+1] << "] = " << arg187[arg177[local276+1]] << "\n";
      if (arg187[arg177[local276]] > arg187[arg177[local276 + 1]]) {
        // std::cout << "    Left greater than right; increment local276" <<
        // "\n";
        local276++;
      }
    }

    // std::cout << "  Lookup arg187[local289] = arg187[" << local289 << "] = "
    // << arg187[local289] << "\n"; std::cout << "  Lookup
    // arg187[arg177[local276]] = arg187[" << arg177[local276] << "] = " <<
    // arg187[arg177[local276]] << "\n";

    if (arg187[local289] <= arg187[arg177[local276]]) {
      // std::cout << "  Fin\n";
      break;
    }
    // std::cout << "  Moved " << local276 << " to " << i << "\n";
    arg177[i] = arg177[local276];
    i = local276;
  }
  arg177[i] = (uint16_t)local289;
  // WRITE_ARRAY_PTR(std::cout, data, "arg177", arg177, int16_t);
  // std::cout << "\n";
}
