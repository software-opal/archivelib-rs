
#include "r_expand.hpp"

int32_t RExpand::Expand() {
  int32_t _231;
  int16_t _226;
  int16_t _276;
  int16_t _203;
  int16_t _200;
  uint8_t *l_uncompressed_buffer278;
  int16_t _279;
  int16_t _280;

  data->dat243 = 0;
  data->dat244 = 0;
  data->bits182 = 0;
  data->tmp_bit_buffer245 = 0;
  data->bits_in_buffer172 = 0;
  data->dat246 = 0;

  l_uncompressed_buffer278 = data->uncompressed_buffer;
  _279 = data->dat175;
  _280 = data->dat176;
  _231 = 0;
  _200 = 0;

  fn256(2 * CHAR_BIT);
  while (data->dat243 < 5) {
    if ((_203 = fn249()) <= UCHAR_MAX) {
      l_uncompressed_buffer278[_200] = (uint8_t)_203;
      if (++_200 >= _279) {
        _200 = 0;
        if ((int16_t)data->output_store->WriteBuffer(l_uncompressed_buffer278, _279) != _279)
          goto _282;
      }
    } else {
      _276 = (int16_t)(_203 - (UCHAR_MAX + 1 - CONST_N135_IS_3));
      if (_276 == CONST_N144_IS_257)
        break;
      _226 = (int16_t)((_200 - fn250() - 1) & _280);
      if (_226 < _279 - CONST_N140_IS_256 - 1 &&
          _200 < _279 - CONST_N140_IS_256 - 1) {
        while (--_276 >= 0)
          l_uncompressed_buffer278[_200++] = l_uncompressed_buffer278[_226++];
      } else {
        while (--_276 >= 0) {
          l_uncompressed_buffer278[_200] = l_uncompressed_buffer278[_226];
          if (++_200 >= _279) {
            _200 = 0;
            if ((int16_t)data->output_store->WriteBuffer(l_uncompressed_buffer278, _279) != _279)
              goto _282;
          }
          _226 = (int16_t)((_226 + 1) & _280);
        }
      }
    }
  }
  if (_200 != 0)
    data->output_store->WriteBuffer(l_uncompressed_buffer278, _200);
_282:
  return _231;
}