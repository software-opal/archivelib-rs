
#include <cassert>

#include "support/expand.hpp"

#include "r_expand.hpp"

uint16_t RExpand::get_next_item() {
  uint16_t run_length276, _283;
  if (data->items_until_next_header == 0) {
    // This is the first 2 bytes in the file, and it represents the number of
    // calls that this header can handle. It's not exactly the number of bytes
    // because we read a variable number of bits per call.
    data->items_until_next_header = get_bits(16);
    DE;
    fn253(CONST_N145_IS_19, CONST_N147_IS_5, 3);
    DE;
    fn255();
    DE;
    fn253(CONST_N142_IS_15, CONST_N540_IS_5, -1);
    DE;
    if (mStatus < 0)
      return 0;
  }
  data->items_until_next_header--;
  run_length276 = data->dat_arr240[data->bits182 >> 4];
  // run_length276 <= 0xFF are the uncompressed bits.
  // 0x100 <= run_length276 <= 0x1FE are runs (run_length276 - 0x100 + 3) bits
  // long
  if (run_length276 >= CONST_N141_IS_511) {
    // No test cases exercise this condition.
    printf("\n");
    WRITE_BITS(std::cout, "", data->bits182);
    printf("\n");
    _283 = 1U << 3;
    do {
      printf("                _283: %#4x\n", _283);
      printf("data->bits182 & _283: %#4x\n", data->bits182 & _283);
      printf("       run_length276: %#x\n", run_length276);
      if (data->bits182 & _283)
        run_length276 = data->dat_arr190[run_length276];
      else
        run_length276 = data->dat_arr189[run_length276];
      _283 >>= 1;
    } while (run_length276 >= CONST_N141_IS_511);
    printf("\n");
    abort();
  }
  read_bits(data->dat_arr180[run_length276]);
  return run_length276;
}
