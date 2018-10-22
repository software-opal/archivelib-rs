#include <cstring>

#include "r_expand.hpp"

RExpand::RExpand(ALStorage &_266, ALStorage &_267, long _268, int _269) {
  _161 = &_266;
  _162 = &_267;
  _248 = _268;
  ;
  if (_269 > MAX_COMPRESSION_FACTOR || _269 < MIN_COMPRESSION_FACTOR) {
    mStatus.SetError(AL_ILLEGAL_PARAMETER, INVALID_COMPRESSION_LEVEL_MSG,
                     _269 - 10);
    _175 = 2;
  } else
    _175 = (short)(1 << _269);
  _176 = (short)(_175 - 1);
  _166 = new uchar[_175 + 2];
  if (_166)
    memset(_166, 0, (_175 + 2) * sizeof(uchar));
  _240 = new ushort[CONST_N148_IS_4096];
  if (_240)
    memset(_240, 0, CONST_N148_IS_4096 * sizeof(ushort));
  _241 = new ushort[CONST_N149_IS_256];
  if (_241)
    memset(_241, 0, CONST_N149_IS_256 * sizeof(ushort));
  _242 = new uchar[BUFFER_SIZE];
  if (_242)
    memset(_242, 0, BUFFER_SIZE * sizeof(uchar));
  _189 = new ushort[2 * CONST_N141_IS_511 - 1];
  if (_189)
    memset(_189, 0, (2 * CONST_N141_IS_511 - 1) * sizeof(ushort));
  _190 = new ushort[2 * CONST_N141_IS_511 - 1];
  if (_190)
    memset(_190, 0, (2 * CONST_N141_IS_511 - 1) * sizeof(ushort));
  _180 = new uchar[CONST_N141_IS_511];
  _181 = new uchar[CONST_N152_IS_19];
  if (!_166 || !_240 || !_241 || !_242 || !_189 || !_190 || !_180 || !_181) {
    mStatus.SetError(AL_CANT_ALLOCATE_MEMORY, MEMORY_ALLOCATION_FAILURE_MSG);
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
int RExpand::Expand() {
  int _231;
  short _226;
  short _276;
  short _203;
  short _200;
  uchar *_278;
  short _279;
  short _280;
  _278 = _166;
  _279 = _175;
  _280 = _176;
  _231 = 0;
  _243 = 0;
  fn251();
  _200 = 0;
  while (_243 < 5) {
    if ((_203 = fn249()) <= UCHAR_MAX) {
      _278[_200] = (uchar)_203;
      if (++_200 >= _279) {
        _200 = 0;
        if ((short)_162->WriteBuffer(_278, _279) != _279)
          goto _282;
      }
    } else {
      _276 = (short)(_203 - (UCHAR_MAX + 1 - CONST_N135_IS_3));
      if (_276 == CONST_N144_IS_257)
        break;
      _226 = (short)((_200 - fn250() - 1) & _280);
      if (_226 < _279 - CONST_N140_IS_256 - 1 &&
          _200 < _279 - CONST_N140_IS_256 - 1) {
        while (--_276 >= 0)
          _278[_200++] = _278[_226++];
      } else {
        while (--_276 >= 0) {
          _278[_200] = _278[_226];
          if (++_200 >= _279) {
            _200 = 0;
            if ((short)_162->WriteBuffer(_278, _279) != _279)
              goto _282;
          }
          _226 = (short)((_226 + 1) & _280);
        }
      }
    }
  }
  if (_200 != 0)
    _162->WriteBuffer(_278, _200);
_282:
  return _231;
}
ushort RExpand::fn249() {
  ushort _276, _283;
  if (_244 == 0) {
    _244 = fn252(16);
    fn253(CONST_N145_IS_19, CONST_N147_IS_5, 3);
    fn255();
    fn253(CONST_N142_IS_15, CONST_N540_IS_5, -1);
    if (mStatus < 0)
      return 0;
  }
  _244--;
  _276 = _240[_182 >> 4];
  if (_276 >= CONST_N141_IS_511) {
    _283 = 1U << 3;
    do {
      if (_182 & _283)
        _276 = _190[_276];
      else
        _276 = _189[_276];
      _283 >>= 1;
    } while (_276 >= CONST_N141_IS_511);
  }
  fn256(_180[_276]);
  return _276;
}
ushort RExpand::fn250() {
  ushort _276, _283;
  _276 = _241[_182 >> 8];
  if (_276 >= CONST_N142_IS_15) {
    _283 = 1U << 7;
    do {
      if (_182 & _283)
        _276 = _190[_276];
      else
        _276 = _189[_276];
      _283 >>= 1;
    } while (_276 >= CONST_N142_IS_15);
  }
  fn256(_181[_276]);
  if (_276 != 0) {
    _276--;
    _276 = (short)((1U << _276) + fn252(_276));
  }
  return _276;
}
void RExpand::fn251() {
  _244 = 0;
  fn257();
}
ushort RExpand::fn252(int _219) {
  ushort _284;
  _284 = (ushort)(_182 >> (2 * CHAR_BIT - _219));
  fn256(_219);
  return _284;
}
void RExpand::fn253(short _254, short _220, short _221) {
  short _226, _203, _219;
  ushort _283;
  _219 = fn252(_220);
  if (_219 == 0) {
    _203 = fn252(_220);
    for (_226 = 0; _226 < _254; _226++)
      _181[_226] = 0;
    for (_226 = 0; _226 < 256; _226++)
      _241[_226] = _203;
  } else {
    _226 = 0;
    while (_226 < _219) {
      _203 = (short)(_182 >> 13);
      if (_203 == 7) {
        _283 = 1U << 12;
        while (_283 & _182) {
          _283 >>= 1;
          _203++;
        }
      }
      fn256((_203 < 7) ? 3 : _203 - 3);
      _181[_226++] = (uchar)_203;
      if (_226 == _221) {
        _203 = fn252(2);
        while (--_203 >= 0)
          _181[_226++] = 0;
      }
    }
    while (_226 < _254)
      _181[_226++] = 0;
    fn258(_254, _181, 8, _241, CONST_N149_IS_256);
  }
}
void RExpand::fn255() {
  short _226, _203, _219;
  ushort _283;
  _219 = fn252(CONST_N143_IS_9);
  if (_219 == 0) {
    _203 = fn252(CONST_N143_IS_9);
    for (_226 = 0; _226 < CONST_N141_IS_511; _226++)
      _180[_226] = 0;
    for (_226 = 0; _226 < CONST_N148_IS_4096; _226++)
      _240[_226] = _203;
  } else {
    _226 = 0;
    while (_226 < _219) {
      _203 = _241[_182 >> 8];
      if (_203 >= CONST_N145_IS_19) {
        _283 = 1U << 7;
        do {
          if (_182 & _283)
            _203 = _190[_203];
          else
            _203 = _189[_203];
          _283 >>= 1;
        } while (_203 >= CONST_N145_IS_19);
      }
      fn256(_181[_203]);
      if (_203 <= 2) {
        if (_203 == 0)
          _203 = 1;
        else if (_203 == 1)
          _203 = (short)(fn252(4) + 3);
        else
          _203 = (short)(fn252(CONST_N143_IS_9) + 20);
        while (--_203 >= 0)
          _180[_226++] = 0;
      } else
        _180[_226++] = (uchar)(_203 - 2);
    }
    while (_226 < CONST_N141_IS_511)
      _180[_226++] = 0;
    fn258(CONST_N141_IS_511, _180, 12, _240, CONST_N148_IS_4096);
  }
}
void RExpand::fn256(int _219) {
  while (_219 > _172) {
    _219 -= _172;
    _182 = (ushort)((_182 << _172) + (_245 >> (CHAR_BIT - _172)));
    if (_246 <= 0) {
      _247 = _242;
      if (_248 >= 0 && _248 < BUFFER_SIZE) {
        _246 = (short)_161->ReadBuffer(_242, (size_t)_248);
        _248 -= _246;
      } else
        _246 = (short)_161->ReadBuffer(_242, BUFFER_SIZE);
      if (_246 <= 0)
        _243++;
    }
    _245 = *_247++;
    _246--;
    _172 = CHAR_BIT;
  }
  _172 = (short)(_172 - _219);
  _182 = (ushort)((_182 << _219) + (_245 >> (CHAR_BIT - _219)));
  _245 <<= _219;
}
void RExpand::fn257() {
  _182 = 0;
  _245 = 0;
  _172 = 0;
  _246 = 0;
  fn256(2 * CHAR_BIT);
}
#if defined(AL_BORLAND) && defined(AL_FLAT_MODEL)
#pragma option -Od
#endif
void RExpand::fn258(int _259, uchar *_260, int _261, ushort *_262,
                    ushort _263) {
  ushort _277[17], _287[17], _288[18], *_204;
  uint _226, _289, _209, _290, _291, _292, _293, _283;
  for (_226 = 1; _226 <= 16; _226++)
    _277[_226] = 0;
  for (_226 = 0; (int)_226 < _259; _226++)
    _277[_260[_226]]++;
  _288[1] = 0;
  for (_226 = 1; _226 <= 16; _226++)
    _288[_226 + 1] = (ushort)(_288[_226] + (_277[_226] << (16 - _226)));
  if (_288[17] != (ushort)(1U << 16)) {
    mStatus.SetError(AL_INTERNAL_ERROR, INTERNAL_ERROR_1_MSG);
    _243 = 10;
    return;
  }
  _291 = 16 - _261;
  for (_226 = 1; (int)_226 <= _261; _226++) {
    _288[_226] >>= _291;
    _287[_226] = (ushort)(1U << (_261 - _226));
  }
  while (_226 <= 16) {
    _287[_226] = (ushort)(1U << (16 - _226));
    _226++;
  }
  _226 = _288[_261 + 1] >> _291;
  if (_226 != (ushort)(1U << 16)) {
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
        _243 = 10;
        return;
      }
      for (_226 = _288[_209]; _226 < _293; _226++)
        _262[_226] = (ushort)_290;
    } else {
      _289 = _288[_209];
      _204 = &_262[_289 >> _291];
      _226 = _209 - _261;
      while (_226 != 0) {
        if (*_204 == 0) {
          _190[_292] = _189[_292] = 0;
          *_204 = (ushort)_292++;
        }
        if (_289 & _283)
          _204 = &_190[*_204];
        else
          _204 = &_189[*_204];
        _289 <<= 1;
        _226--;
      }
      *_204 = (ushort)_290;
    }
    _288[_209] = (ushort)_293;
  }
}
