
#include "r_expand.hpp"

void RExpand::fn253(int16_t _254, int16_t _220, int16_t _221) {
  int16_t run_start226, byte_or_run_length203, bits_to_load219;
  uint16_t _283;
  bits_to_load219 = get_bits(_220);
  if (bits_to_load219 == 0) {
    byte_or_run_length203 = get_bits(_220);
    for (run_start226 = 0; run_start226 < _254; run_start226++) {
      data->dat_arr181[run_start226] = 0;
    }
    for (run_start226 = 0; run_start226 < 256; run_start226++) {
      data->dat_arr241[run_start226] = byte_or_run_length203;
    }
  } else {
    run_start226 = 0;
    while (run_start226 < bits_to_load219) {
      byte_or_run_length203 = (int16_t)(data->bits182 >> 13);
      if (byte_or_run_length203 == 7) {
        size_t bytes_read = 3;
        _283 = 1U << 12;
        while (_283 & data->bits182) {
          _283 >>= 1;
          byte_or_run_length203++;
          bytes_read++;
        }
        // +1 for the final bit that was zero
        read_bits(bytes_read + 1);
      } else {
        read_bits(3);
      }
      data->dat_arr181[run_start226++] = (uint8_t)byte_or_run_length203;
      if (run_start226 == _221) {
        byte_or_run_length203 = get_bits(2);
        while (--byte_or_run_length203 >= 0) {
          data->dat_arr181[run_start226++] = 0;
        }
      }
    }
    while (run_start226 < _254) {
      data->dat_arr181[run_start226++] = 0;
    }
    fn258(_254, data->dat_arr181, 8, data->dat_arr241, CONST_N149_IS_256);
  }
}
