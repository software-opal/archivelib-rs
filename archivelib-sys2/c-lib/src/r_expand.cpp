#include <cstring>

#include "r_expand.hpp"

RExpand::~RExpand() {
  if (data->dat166)
    delete[] data->dat166;
  if (data->dat240)
    delete[] data->dat240;
  if (data->dat241)
    delete[] data->dat241;
  if (data->dat242)
    delete[] data->dat242;
  if (data->dat189)
    delete[] data->dat189;
  if (data->dat190)
    delete[] data->dat190;
  if (data->dat180)
    delete[] data->dat180;
  if (data->dat181)
    delete[] data->dat181;
}
int RExpand::Expand() {
  int _231;
  int16_t _226;
  int16_t _276;
  int16_t _203;
  int16_t _200;
  uint8_t *_278;
  int16_t _279;
  int16_t _280;
  _278 = data->dat166;
  _279 = data->dat175;
  _280 = data->dat176;
  _231 = 0;
  data->dat243 = 0;
  fn251();
  _200 = 0;
  while (data->dat243 < 5) {
    if ((_203 = fn249()) <= UCHAR_MAX) {
      _278[_200] = (uint8_t)_203;
      if (++_200 >= _279) {
        _200 = 0;
        if ((int16_t)data->dat162->WriteBuffer(_278, _279) != _279)
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
          _278[_200++] = _278[_226++];
      } else {
        while (--_276 >= 0) {
          _278[_200] = _278[_226];
          if (++_200 >= _279) {
            _200 = 0;
            if ((int16_t)data->dat162->WriteBuffer(_278, _279) != _279)
              goto _282;
          }
          _226 = (int16_t)((_226 + 1) & _280);
        }
      }
    }
  }
  if (_200 != 0)
    data->dat162->WriteBuffer(_278, _200);
_282:
  return _231;
}
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
  _276 = data->dat240[data->dat182 >> 4];
  if (_276 >= CONST_N141_IS_511) {
    _283 = 1U << 3;
    do {
      if (data->dat182 & _283)
        _276 = data->dat190[_276];
      else
        _276 = data->dat189[_276];
      _283 >>= 1;
    } while (_276 >= CONST_N141_IS_511);
  }
  fn256(data->dat180[_276]);
  return _276;
}
uint16_t RExpand::fn250() {
  uint16_t _276, _283;
  _276 = data->dat241[data->dat182 >> 8];
  if (_276 >= CONST_N142_IS_15) {
    _283 = 1U << 7;
    do {
      if (data->dat182 & _283)
        _276 = data->dat190[_276];
      else
        _276 = data->dat189[_276];
      _283 >>= 1;
    } while (_276 >= CONST_N142_IS_15);
  }
  fn256(data->dat181[_276]);
  if (_276 != 0) {
    _276--;
    _276 = (int16_t)((1U << _276) + fn252(_276));
  }
  return _276;
}
void RExpand::fn251() {
  data->dat244 = 0;
  fn257();
}
uint16_t RExpand::fn252(int _219) {
  uint16_t _284;
  _284 = (uint16_t)(data->dat182 >> (2 * CHAR_BIT - _219));
  fn256(_219);
  return _284;
}
void RExpand::fn253(int16_t _254, int16_t _220, int16_t _221) {
  int16_t _226, _203, _219;
  uint16_t _283;
  _219 = fn252(_220);
  if (_219 == 0) {
    _203 = fn252(_220);
    for (_226 = 0; _226 < _254; _226++)
      data->dat181[_226] = 0;
    for (_226 = 0; _226 < 256; _226++)
      data->dat241[_226] = _203;
  } else {
    _226 = 0;
    while (_226 < _219) {
      _203 = (int16_t)(data->dat182 >> 13);
      if (_203 == 7) {
        _283 = 1U << 12;
        while (_283 & data->dat182) {
          _283 >>= 1;
          _203++;
        }
      }
      fn256((_203 < 7) ? 3 : _203 - 3);
      data->dat181[_226++] = (uint8_t)_203;
      if (_226 == _221) {
        _203 = fn252(2);
        while (--_203 >= 0)
          data->dat181[_226++] = 0;
      }
    }
    while (_226 < _254)
      data->dat181[_226++] = 0;
    fn258(_254, data->dat181, 8, data->dat241, CONST_N149_IS_256);
  }
}
void RExpand::fn255() {
  int16_t _226, _203, _219;
  uint16_t _283;
  _219 = fn252(CONST_N143_IS_9);
  if (_219 == 0) {
    _203 = fn252(CONST_N143_IS_9);
    for (_226 = 0; _226 < CONST_N141_IS_511; _226++)
      data->dat180[_226] = 0;
    for (_226 = 0; _226 < CONST_N148_IS_4096; _226++)
      data->dat240[_226] = _203;
  } else {
    _226 = 0;
    while (_226 < _219) {
      _203 = data->dat241[data->dat182 >> 8];
      if (_203 >= CONST_N145_IS_19) {
        _283 = 1U << 7;
        do {
          if (data->dat182 & _283)
            _203 = data->dat190[_203];
          else
            _203 = data->dat189[_203];
          _283 >>= 1;
        } while (_203 >= CONST_N145_IS_19);
      }
      fn256(data->dat181[_203]);
      if (_203 <= 2) {
        if (_203 == 0)
          _203 = 1;
        else if (_203 == 1)
          _203 = (int16_t)(fn252(4) + 3);
        else
          _203 = (int16_t)(fn252(CONST_N143_IS_9) + 20);
        while (--_203 >= 0)
          data->dat180[_226++] = 0;
      } else
        data->dat180[_226++] = (uint8_t)(_203 - 2);
    }
    while (_226 < CONST_N141_IS_511)
      data->dat180[_226++] = 0;
    fn258(CONST_N141_IS_511, data->dat180, 12, data->dat240,
          CONST_N148_IS_4096);
  }
}
void RExpand::fn256(int _219) {
  while (_219 > data->dat172) {
    _219 -= data->dat172;
    data->dat182 = (uint16_t)((data->dat182 << data->dat172) +
                            (data->dat245 >> (CHAR_BIT - data->dat172)));
    if (data->dat246 <= 0) {
      data->dat247 = data->dat242;
      if (data->dat248 >= 0 && data->dat248 < BUFFER_SIZE) {
        data->dat246 =
            (int16_t)data->dat161->ReadBuffer(data->dat242, (ssize_t)data->dat248);
        data->dat248 -= data->dat246;
      } else
        data->dat246 =
            (int16_t)data->dat161->ReadBuffer(data->dat242, BUFFER_SIZE);
      if (data->dat246 <= 0)
        data->dat243++;
    }
    data->dat245 = *data->dat247++;
    data->dat246--;
    data->dat172 = CHAR_BIT;
  }
  data->dat172 = (int16_t)(data->dat172 - _219);
  data->dat182 =
      (uint16_t)((data->dat182 << _219) + (data->dat245 >> (CHAR_BIT - _219)));
  data->dat245 <<= _219;
}
void RExpand::fn257() {
  data->dat182 = 0;
  data->dat245 = 0;
  data->dat172 = 0;
  data->dat246 = 0;
  fn256(2 * CHAR_BIT);
}
#if defined(AL_BORLAND) && defined(AL_FLAT_MODEL)
#pragma option -Od
#endif
void RExpand::fn258(int _259, uint8_t *_260, int _261, uint16_t *_262,
                    uint16_t _263) {
  uint16_t _277[17], _287[17], _288[18], *_204;
  uint _226, _289, _209, _290, _291, _292, _293, _283;
  for (_226 = 1; _226 <= 16; _226++)
    _277[_226] = 0;
  for (_226 = 0; (int)_226 < _259; _226++)
    _277[_260[_226]]++;
  _288[1] = 0;
  for (_226 = 1; _226 <= 16; _226++)
    _288[_226 + 1] = (uint16_t)(_288[_226] + (_277[_226] << (16 - _226)));
  if (_288[17] != (uint16_t)(1U << 16)) {
    mStatus.SetError(AL_INTERNAL_ERROR, INTERNAL_ERROR_1_MSG);
    data->dat243 = 10;
    return;
  }
  _291 = 16 - _261;
  for (_226 = 1; (int)_226 <= _261; _226++) {
    _288[_226] >>= _291;
    _287[_226] = (uint16_t)(1U << (_261 - _226));
  }
  while (_226 <= 16) {
    _287[_226] = (uint16_t)(1U << (16 - _226));
    _226++;
  }
  _226 = _288[_261 + 1] >> _291;
  if (_226 != (uint16_t)(1U << 16)) {
    _289 = 1U << _261;
    while (_226 != _289)
      _262[_226++] = 0;
  }
  _292 = _259;
  _283 = 1U << (15 - _261);
  for (_290 = 0; (int)_290 < _259; _290++) {
    if ((_209 = _260[_290]) == 0)
      continue;
    _293 = _288[_209] + _287[_209];
    if ((int)_209 <= _261) {
      if (_293 > _263) {
        mStatus.SetError(AL_INTERNAL_ERROR, INTERNAL_ERROR_2_MSG);
        data->dat243 = 10;
        return;
      }
      for (_226 = _288[_209]; _226 < _293; _226++)
        _262[_226] = (uint16_t)_290;
    } else {
      _289 = _288[_209];
      _204 = &_262[_289 >> _291];
      _226 = _209 - _261;
      while (_226 != 0) {
        if (*_204 == 0) {
          data->dat190[_292] = data->dat189[_292] = 0;
          *_204 = (uint16_t)_292++;
        }
        if (_289 & _283)
          _204 = &data->dat190[*_204];
        else
          _204 = &data->dat189[*_204];
        _289 <<= 1;
        _226--;
      }
      *_204 = (uint16_t)_290;
    }
    _288[_209] = (uint16_t)_293;
  }
}
