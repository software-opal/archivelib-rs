
#include <assert.h>

#include "support/expand.h"

#include "r_expand.hpp"

uint16_t get_next_item(RExpandData *data) {
  uint16_t run_length276, _283;
  if (data->items_until_next_header == 0) {
    // This is the first 2 bytes in the file, and it represents the number of
    // calls that this header can handle. It's not exactly the number of bytes
    // because we read a variable number of bits per call.
    data->items_until_next_header = get_bits(data, 16);
    DE;
    fn253(data, CONST_N145_IS_19, CONST_N147_IS_5, 3);
    fn255(data);
    fn253(data, CONST_N142_IS_15, CONST_N540_IS_5, -1);
    DE;
    if (data->error)
      return 0;
  }
  if (data->items_until_next_header == 0) {
    // printf("No items until next header!? -- %i\n", (uint16_t)
    // (data->items_until_next_header - 1));
  }
  // printf("!%i!\n", data->items_until_next_header);
  data->items_until_next_header--;

  run_length276 = data->dat_arr240[data->bits182 >> 4];
  // run_length276 <= 0xFF are the uncompressed bits.
  // 0x100 <= run_length276 <= 0x1FE are runs (run_length276 - 0x100 + 3) bits
  // long
  if (run_length276 >= CONST_N141_IS_511) {
    // No test cases exercise this condition.
    _283 = 1U << 3;
    do {
      if (data->bits182 & _283)
        run_length276 = data->dat_arr190[run_length276];
      else
        run_length276 = data->dat_arr189[run_length276];
      _283 >>= 1;
    } while (run_length276 >= CONST_N141_IS_511);
  }
  // printf("!%i! --- %x / %i / %i / %i\n", data->items_until_next_header,
  // data->bits182, data->dat_arr240[data->bits182 >> 4], run_length276,
  // data->dat_arr180[run_length276]);
  read_bits(data, data->dat_arr180[run_length276]);
  return run_length276;
}
