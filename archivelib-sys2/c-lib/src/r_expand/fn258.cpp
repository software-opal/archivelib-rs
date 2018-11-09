#include "r_expand.hpp"

void RExpand::fn258(int32_t _259, uint8_t *_260, int32_t _261, uint16_t *_262,
                    uint16_t _263) {
  uint16_t _277[17], _287[17], _288[18], *_204;
  uint32_t run_start226, _289, _209, _290, _291, _292, _293, _283;
  for (run_start226 = 1; run_start226 <= 16; run_start226++)
    _277[run_start226] = 0;
  for (run_start226 = 0; run_start226 < _259; run_start226++)
    _277[_260[run_start226]]++;
  _288[1] = 0;
  for (run_start226 = 1; run_start226 <= 16; run_start226++)
    _288[run_start226 + 1] =
        _288[run_start226] + (_277[run_start226] << (16 - run_start226));
  if (_288[17] != (uint16_t)(1U << 16)) {
    mStatus.SetError(AL_INTERNAL_ERROR, INTERNAL_ERROR_1_MSG);
    data->error_counter243 = 10;
    return;
  }
  _291 = 16 - _261;
  for (run_start226 = 1; (int32_t)run_start226 <= _261; run_start226++) {
    _288[run_start226] >>= _291;
    _287[run_start226] = (uint16_t)(1U << (_261 - run_start226));
  }
  while (run_start226 <= 16) {
    _287[run_start226] = (uint16_t)(1U << (16 - run_start226));
    run_start226++;
  }
  run_start226 = _288[_261 + 1] >> _291;
  if (run_start226 != (uint16_t)(1U << 16)) {
    _289 = 1U << _261;
    while (run_start226 != _289)
      _262[run_start226++] = 0;
  }
  _292 = _259;
  _283 = 1U << (15 - _261);
  for (_290 = 0; _290 < _259; _290++) {
    if ((_209 = _260[_290]) == 0)
      continue;
    _293 = _288[_209] + _287[_209];
    if (_209 <= _261) {
      if (_293 > _263) {
        mStatus.SetError(AL_INTERNAL_ERROR, INTERNAL_ERROR_2_MSG);
        data->error_counter243 = 10;
        return;
      }
      for (run_start226 = _288[_209]; run_start226 < _293; run_start226++)
        _262[run_start226] = (uint16_t)_290;
    } else {
      _289 = _288[_209];
      _204 = &_262[_289 >> _291];
      run_start226 = _209 - _261;
      while (run_start226 != 0) {
        if (*_204 == 0) {
          data->dat_arr190[_292] = data->dat_arr189[_292] = 0;
          *_204 = (uint16_t)_292++;
        }
        if (_289 & _283)
          _204 = &data->dat_arr190[*_204];
        else
          _204 = &data->dat_arr189[*_204];
        _289 <<= 1;
        run_start226--;
      }
      *_204 = (uint16_t)_290;
    }
    _288[_209] = (uint16_t)_293;
  }
}
