
#include <cassert>

#include "support/expand.hpp"

#include "r_expand.hpp"
// len(data->dat_arr180) == 511

void seed_expand(RExpandData *data) {
  expand_get_bits(data, 5);
  expand_get_bits(data, 3);
  expand_get_bits(data, 3);
  expand_get_bits(data, 3);
  expand_get_bits(data, 2);
  expand_get_bits(data, 3);

  expand_get_bits(data, 9);
  expand_get_bits(data, 1);
  expand_get_bits(data, 1);
  expand_get_bits(data, 9);
  expand_get_bits(data, 1);

  expand_get_bits(data, 5);
  expand_get_bits(data, 5);

  data->dat_arr180[0] = 1;
  data->dat_arr180[510] = 1;

  for (size_t i = 2048; i < 4096; i++) {
    data->dat_arr240[i] = 510;
  }
}
