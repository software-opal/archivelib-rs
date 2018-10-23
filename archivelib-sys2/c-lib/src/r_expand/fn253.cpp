
#include "r_expand.hpp"

void RExpand::fn253(int16_t _254, int16_t _220, int16_t _221) {
  int16_t _226, _203, bits_to_load219;
  uint16_t _283;
  bits_to_load219 = fn252(_220);
  if (bits_to_load219 == 0) {
    _203 = fn252(_220);
    for (_226 = 0; _226 < _254; _226++)
      data->dat_arr181[_226] = 0;
    for (_226 = 0; _226 < 256; _226++)
      data->dat_arr241[_226] = _203;
  } else {
    _226 = 0;
    while (_226 < bits_to_load219) {
      _203 = (int16_t)(data->bits182 >> 13);
      if (_203 == 7) {
        _283 = 1U << 12;
        while (_283 & data->bits182) {
          _283 >>= 1;
          _203++;
        }
      }
      fn256((_203 < 7) ? 3 : _203 - 3);
      data->dat_arr181[_226++] = (uint8_t)_203;
      if (_226 == _221) {
        _203 = fn252(2);
        while (--_203 >= 0)
          data->dat_arr181[_226++] = 0;
      }
    }
    while (_226 < _254)
      data->dat_arr181[_226++] = 0;
    fn258(_254, data->dat_arr181, 8, data->dat_arr241, CONST_N149_IS_256);
  }
}
