
#include "r_expand.hpp"

void RExpand::fn255() {
  int16_t run_start226, byte_or_run_length203, bits_to_load219;
  uint16_t _283;
  bits_to_load219 = fn252(CONST_N143_IS_9);
  if (bits_to_load219 == 0) {
    byte_or_run_length203 = fn252(CONST_N143_IS_9);
    for (run_start226 = 0; run_start226 < CONST_N141_IS_511; run_start226++)
      data->dat_arr180[run_start226] = 0;
    for (run_start226 = 0; run_start226 < CONST_N148_IS_4096; run_start226++)
      data->dat_arr240[run_start226] = byte_or_run_length203;
  } else {
    run_start226 = 0;
    while (run_start226 < bits_to_load219) {
      byte_or_run_length203 = data->dat_arr241[data->bits182 >> 8];
      if (byte_or_run_length203 >= CONST_N145_IS_19) {
        _283 = 1U << 7;
        do {
          if (data->bits182 & _283)
            byte_or_run_length203 = data->dat_arr190[byte_or_run_length203];
          else
            byte_or_run_length203 = data->dat_arr189[byte_or_run_length203];
          _283 >>= 1;
        } while (byte_or_run_length203 >= CONST_N145_IS_19);
      }
      read_bits(data->dat_arr181[byte_or_run_length203]);
      if (byte_or_run_length203 <= 2) {
        if (byte_or_run_length203 == 0)
          byte_or_run_length203 = 1;
        else if (byte_or_run_length203 == 1)
          byte_or_run_length203 = (int16_t)(fn252(4) + 3);
        else
          byte_or_run_length203 = (int16_t)(fn252(CONST_N143_IS_9) + 20);
        while (--byte_or_run_length203 >= 0)
          data->dat_arr180[run_start226++] = 0;
      } else
        data->dat_arr180[run_start226++] = (uint8_t)(byte_or_run_length203 - 2);
    }
    while (run_start226 < CONST_N141_IS_511)
      data->dat_arr180[run_start226++] = 0;
    fn258(CONST_N141_IS_511, data->dat_arr180, 12, data->dat_arr240,
          CONST_N148_IS_4096);
  }
}
