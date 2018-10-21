#include "arclib.h"

#include "_re.hpp"
#include <cstring>

RExpand::RExpand(ALStorage &_266, ALStorage &_267, int64_t _268, int32_t _269) {
  _161 = &_266;
  _162 = &_267;
  _248 = _268;
  ;
  if (_269 > CONST_N137 || _269 < CONST_N138) {
    mStatus.SetError(AL_ILLEGAL_PARAMETER, ERROR_MESSAGE_N519, _269 - 10);
    _175 = 2;
  } else
    _175 = (int16_t)(1 << _269);
  _176 = (int16_t)(_175 - 1);
  _166 = new uint8_t[_175 + 2];
  if (_166)
    memset(_166, 0, (_175 + 2) * sizeof(uint8_t));
  _240 = new uint16_t[CONST_N148];
  if (_240)
    memset(_240, 0, CONST_N148 * sizeof(uint16_t));
  _241 = new uint16_t[CONST_N149];
  if (_241)
    memset(_241, 0, CONST_N149 * sizeof(uint16_t));
  _242 = new uint8_t[BUFFER_SIZE];
  if (_242)
    memset(_242, 0, BUFFER_SIZE * sizeof(uint8_t));
  _189 = new uint16_t[2 * CONST_N141 - 1];
  if (_189)
    memset(_189, 0, (2 * CONST_N141 - 1) * sizeof(uint16_t));
  _190 = new uint16_t[2 * CONST_N141 - 1];
  if (_190)
    memset(_190, 0, (2 * CONST_N141 - 1) * sizeof(uint16_t));
  _180 = new uint8_t[CONST_N141];
  _181 = new uint8_t[CONST_N152];
  if (!_166 || !_240 || !_241 || !_242 || !_189 || !_190 || !_180 || !_181) {
    mStatus.SetError(AL_CANT_ALLOCATE_MEMORY, ERROR_MESSAGE_N520);
  }
}
RExpand::~RExpand() {
  if (_166)
    delete[] _166;
  if (_240)
    delete[] _240;
  if (_241)
    delete[] _241;
  if (_242)
    delete[] _242;
  if (_189)
    delete[] _189;
  if (_190)
    delete[] _190;
  if (_180)
    delete[] _180;
  if (_181)
    delete[] _181;
}
int32_t RExpand::Expand() {
  int32_t _231;
  int16_t i;
  int16_t _276;
  int16_t _203;
  int16_t _200;
  uint8_t *_278;
  int16_t _279;
  int16_t _280;
  _278 = _166;
  _279 = _175;
  _280 = _176;
  _231 = 0;
  _243 = 0;
  fn251();
  _200 = 0;
  while (_243 < 5) {
    if ((_203 = _249()) <= UCHAR_MAX) {
      _278[_200] = (uint8_t)_203;
      if (++_200 >= _279) {
        _200 = 0;
        if ((int16_t)_162->WriteBuffer(_278, _279) != _279)
          goto _282;
      }
    } else {
      _276 = (int16_t)(_203 - (UCHAR_MAX + 1 - CONST_N135));
      if (_276 == CONST_N144)
        break;
      i = (int16_t)((_200 - _250() - 1) & _280);
      if (i < _279 - CONST_N140 - 1 && _200 < _279 - CONST_N140 - 1) {
        while (--_276 >= 0)
          _278[_200++] = _278[i++];
      } else {
        while (--_276 >= 0) {
          _278[_200] = _278[i];
          if (++_200 >= _279) {
            _200 = 0;
            if ((int16_t)_162->WriteBuffer(_278, _279) != _279)
              goto _282;
          }
          i = (int16_t)((i + 1) & _280);
        }
      }
    }
  }
  if (_200 != 0)
    _162->WriteBuffer(_278, _200);
_282:
  return _231;
}
uint16_t RExpand::_249() {
  uint16_t _276, _283;
  if (_244 == 0) {
    _244 = fn252(16);
    fn253(CONST_N145, CONST_N147, 3);
    fn255();
    fn253(CONST_N142, CONST_N540, -1);
    if (mStatus < 0)
      return 0;
  }
  _244--;
  _276 = _240[_182 >> 4];
  if (_276 >= CONST_N141) {
    _283 = 1U << 3;
    do {
      if (_182 & _283)
        _276 = _190[_276];
      else
        _276 = _189[_276];
      _283 >>= 1;
    } while (_276 >= CONST_N141);
  }
  fn256(_180[_276]);
  return _276;
}
uint16_t RExpand::_250() {
  uint16_t _276, _283;
  _276 = _241[_182 >> 8];
  if (_276 >= CONST_N142) {
    _283 = 1U << 7;
    do {
      if (_182 & _283)
        _276 = _190[_276];
      else
        _276 = _189[_276];
      _283 >>= 1;
    } while (_276 >= CONST_N142);
  }
  fn256(_181[_276]);
  if (_276 != 0) {
    _276--;
    _276 = (int16_t)((1U << _276) + fn252(_276));
  }
  return _276;
}
void RExpand::fn251() {
  _244 = 0;
  fn257();
}
uint16_t RExpand::fn252(int32_t _219) {
  uint16_t _284;
  _284 = (uint16_t)(_182 >> (2 * CHAR_BIT - _219));
  fn256(_219);
  return _284;
}
void RExpand::fn253(int16_t _254, int16_t _220, int16_t _221) {
  int16_t i, _203, _219;
  uint16_t _283;
  _219 = fn252(_220);
  if (_219 == 0) {
    _203 = fn252(_220);
    for (i = 0; i < _254; i++)
      _181[i] = 0;
    for (i = 0; i < 256; i++)
      _241[i] = _203;
  } else {
    i = 0;
    while (i < _219) {
      _203 = (int16_t)(_182 >> 13);
      if (_203 == 7) {
        _283 = 1U << 12;
        while (_283 & _182) {
          _283 >>= 1;
          _203++;
        }
      }
      fn256((_203 < 7) ? 3 : _203 - 3);
      _181[i++] = (uint8_t)_203;
      if (i == _221) {
        _203 = fn252(2);
        while (--_203 >= 0)
          _181[i++] = 0;
      }
    }
    while (i < _254)
      _181[i++] = 0;
    fn258(_254, _181, 8, _241, CONST_N149);
  }
}
void RExpand::fn255() {
  int16_t i, _203, _219;
  uint16_t _283;
  _219 = fn252(CONST_N143);
  if (_219 == 0) {
    _203 = fn252(CONST_N143);
    for (i = 0; i < CONST_N141; i++)
      _180[i] = 0;
    for (i = 0; i < CONST_N148; i++)
      _240[i] = _203;
  } else {
    i = 0;
    while (i < _219) {
      _203 = _241[_182 >> 8];
      if (_203 >= CONST_N145) {
        _283 = 1U << 7;
        do {
          if (_182 & _283)
            _203 = _190[_203];
          else
            _203 = _189[_203];
          _283 >>= 1;
        } while (_203 >= CONST_N145);
      }
      fn256(_181[_203]);
      if (_203 <= 2) {
        if (_203 == 0)
          _203 = 1;
        else if (_203 == 1)
          _203 = (int16_t)(fn252(4) + 3);
        else
          _203 = (int16_t)(fn252(CONST_N143) + 20);
        while (--_203 >= 0)
          _180[i++] = 0;
      } else
        _180[i++] = (uint8_t)(_203 - 2);
    }
    while (i < CONST_N141)
      _180[i++] = 0;
    fn258(CONST_N141, _180, 12, _240, CONST_N148);
  }
}
void RExpand::fn256(int32_t _219) {
  while (_219 > _172) {
    _219 -= _172;
    _182 = (uint16_t)((_182 << _172) + (_245 >> (CHAR_BIT - _172)));
    if (_246 <= 0) {
      _247 = _242;
      if (_248 >= 0 && _248 < BUFFER_SIZE) {
        _246 = (int16_t)_161->ReadBuffer(_242, (size_t)_248);
        _248 -= _246;
      } else
        _246 = (int16_t)_161->ReadBuffer(_242, BUFFER_SIZE);
      if (_246 <= 0)
        _243++;
    }
    _245 = *_247++;
    _246--;
    _172 = CHAR_BIT;
  }
  _172 = (int16_t)(_172 - _219);
  _182 = (uint16_t)((_182 << _219) + (_245 >> (CHAR_BIT - _219)));
  _245 <<= _219;
}
void RExpand::fn257() {
  _182 = 0;
  _245 = 0;
  _172 = 0;
  _246 = 0;
  fn256(2 * CHAR_BIT);
}
void RExpand::fn258(int32_t _259, uint8_t *_260, int32_t _261, uint16_t *_262,
                   uint16_t _263) {
  uint16_t _277[17], _287[17], _288[18], *_204;
  uint32_t i, _289, _209, _290, _291, _292, _293, _283;
  for (i = 1; i <= 16; i++)
    _277[i] = 0;
  for (i = 0; (int32_t)i < _259; i++)
    _277[_260[i]]++;
  _288[1] = 0;
  for (i = 1; i <= 16; i++)
    _288[i + 1] = (uint16_t)(_288[i] + (_277[i] << (16 - i)));
  if (_288[17] != (uint16_t)(1U << 16)) {
    mStatus.SetError(AL_INTERNAL_ERROR, ERROR_MESSAGE_N521);
    _243 = 10;
    return;
  }
  _291 = 16 - _261;
  for (i = 1; (int32_t)i <= _261; i++) {
    _288[i] >>= _291;
    _287[i] = (uint16_t)(1U << (_261 - i));
  }
  while (i <= 16) {
    _287[i] = (uint16_t)(1U << (16 - i));
    i++;
  }
  i = _288[_261 + 1] >> _291;
  if (i != (uint16_t)(1U << 16)) {
    _289 = 1U << _261;
    while (i != _289)
      _262[i++] = 0;
  }
  _292 = _259;
  _283 = 1U << (15 - _261);
  for (_290 = 0; (int32_t)_290 < _259; _290++) {
    if ((_209 = _260[_290]) == 0)
      continue;
    _293 = _288[_209] + _287[_209];
    if ((int32_t)_209 <= _261) {
      if (_293 > _263) {
        mStatus.SetError(AL_INTERNAL_ERROR, ERROR_MESSAGE_N522);
        _243 = 10;
        return;
      }
      for (i = _288[_209]; i < _293; i++)
        _262[i] = (uint16_t)_290;
    } else {
      _289 = _288[_209];
      _204 = &_262[_289 >> _291];
      i = _209 - _261;
      while (i != 0) {
        if (*_204 == 0) {
          _190[_292] = _189[_292] = 0;
          *_204 = (uint16_t)_292++;
        }
        if (_289 & _283)
          _204 = &_190[*_204];
        else
          _204 = &_189[*_204];
        _289 <<= 1;
        i--;
      }
      *_204 = (uint16_t)_290;
    }
    _288[_209] = (uint16_t)_293;
  }
}
