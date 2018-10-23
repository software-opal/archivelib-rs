#include "r_expand.hpp"

uint16_t RExpand::fn249() {
  uint16_t _276, _283;
  if (data->dat244 == 0) {
    data->dat244 = fn252(16);
    fn253(CONST_N145_IS_19, CONST_N147_IS_5, 3);
    fn255();
    fn253(CONST_N142_IS_15, CONST_N540_IS_5, -1);
    if (mStatus < 0)
      return 0;
  }
  data->dat244--;
  _276 = data->dat_arr240[data->bits182 >> 4];
  if (_276 >= CONST_N141_IS_511) {
    _283 = 1U << 3;
    do {
      if (data->bits182 & _283)
        _276 = data->dat_arr190[_276];
      else
        _276 = data->dat_arr189[_276];
      _283 >>= 1;
    } while (_276 >= CONST_N141_IS_511);
  }
  read_bits(data->dat_arr180[_276]);
  return _276;
}
