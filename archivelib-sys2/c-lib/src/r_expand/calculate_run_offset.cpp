#include "r_expand.hpp"

uint16_t RExpand::calculate_run_offset() {
  uint16_t run_length276, _283;
  run_length276 = data->dat_arr241[data->bits182 >> 8];
  if (run_length276 >= CONST_N142_IS_15) {
    _283 = 1U << 7;
    do {
      if (data->bits182 & _283)
        run_length276 = data->dat_arr190[run_length276];
      else
        run_length276 = data->dat_arr189[run_length276];
      _283 >>= 1;
    } while (run_length276 >= CONST_N142_IS_15);
  }
  read_bits(data->dat_arr181[run_length276]);
  if (run_length276 != 0) {
    run_length276--;
    run_length276 = (int16_t)((1U << run_length276) + fn252(run_length276));
  }
  return run_length276;
}
