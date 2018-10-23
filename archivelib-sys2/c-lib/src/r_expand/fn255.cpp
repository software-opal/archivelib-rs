
#include "r_expand.hpp"

void RExpand::fn255() {
  int16_t _226, _203, bits_to_load219;
  uint16_t _283;
  bits_to_load219 = fn252(CONST_N143_IS_9);
  if (bits_to_load219 == 0) {
    _203 = fn252(CONST_N143_IS_9);
    for (_226 = 0; _226 < CONST_N141_IS_511; _226++)
      data->dat_arr180[_226] = 0;
    for (_226 = 0; _226 < CONST_N148_IS_4096; _226++)
      data->dat_arr240[_226] = _203;
  } else {
    _226 = 0;
    while (_226 < bits_to_load219) {
      _203 = data->dat_arr241[data->bits182 >> 8];
      if (_203 >= CONST_N145_IS_19) {
        _283 = 1U << 7;
        do {
          if (data->bits182 & _283)
            _203 = data->dat_arr190[_203];
          else
            _203 = data->dat_arr189[_203];
          _283 >>= 1;
        } while (_203 >= CONST_N145_IS_19);
      }
      read_bits(data->dat_arr181[_203]);
      if (_203 <= 2) {
        if (_203 == 0)
          _203 = 1;
        else if (_203 == 1)
          _203 = (int16_t)(fn252(4) + 3);
        else
          _203 = (int16_t)(fn252(CONST_N143_IS_9) + 20);
        while (--_203 >= 0)
          data->dat_arr180[_226++] = 0;
      } else
        data->dat_arr180[_226++] = (uint8_t)(_203 - 2);
    }
    while (_226 < CONST_N141_IS_511)
      data->dat_arr180[_226++] = 0;
    fn258(CONST_N141_IS_511, data->dat_arr180, 12, data->dat_arr240,
          CONST_N148_IS_4096);
  }
}
