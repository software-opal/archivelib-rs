#include "r_expand.hpp"

uint16_t RExpand::fn250() {
  uint16_t _276, _283;
  _276 = data->dat_arr241[data->bits182 >> 8];
  if (_276 >= CONST_N142_IS_15) {
    _283 = 1U << 7;
    do {
      if (data->bits182 & _283)
        _276 = data->dat_arr190[_276];
      else
        _276 = data->dat_arr189[_276];
      _283 >>= 1;
    } while (_276 >= CONST_N142_IS_15);
  }
  read_bits(data->dat_arr181[_276]);
  if (_276 != 0) {
    _276--;
    _276 = (int16_t)((1U << _276) + fn252(_276));
  }
  return _276;
}
