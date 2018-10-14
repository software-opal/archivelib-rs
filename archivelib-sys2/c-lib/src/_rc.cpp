#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#define MACRO_FN445(arg278, arg200, arg446)                                    \
  ((int16_t)((arg446 << CONST__154) ^ (arg278[arg200 + 2])) & (CONST__153 - 1))
#define MACRO_FN447(arg163, arg164, arg200, arg201)                                \
  {                                                                            \
    int16_t macro_local204;                                                              \
    if ((macro_local204 = arg163[arg201]) != CONST__157)                                   \
      arg164[macro_local204] = arg200;                                                     \
    arg164[arg200] = arg201;                                                     \
    arg163[arg200] = macro_local204;                                                       \
    arg163[arg201] = arg200;                                                     \
  }
#define MACRO_FN448(arg163, arg164, s)                                                         \
  {                                                                            \
    int16_t macro_local204;                                                              \
    if ((macro_local204 = arg164[s]) != CONST__157) {                                      \
      arg164[s] = CONST__157;                                                    \
      arg163[macro_local204] = CONST__157;                                                 \
    }                                                                          \
  }
RCompress::RCompress(ALStorage &_266, ALStorage &_267, int32_t _269,
                     int32_t _235) {
  _161 = &_266;
  _162 = &_267;
  _531 = _235;
  if (_269 > CONST__137 || _269 < CONST__138) {
    mStatus.SetError(AL_ILLEGAL_PARAMETER, _519, _269 - 10);
    _175 = 2;
  } else
    _175 = (int16_t)(1 << _269);
  _176 = (int16_t)(_175 - 1);
  if ((_166 = new uint8_t[_175 + CONST__140 + 2]) != 0)
    memset(_166, 0, (_175 + CONST__140 + 2) * sizeof(uint8_t));
  if ((_163 = new int16_t[_175 + CONST__153]) != 0)
    memset(_163, 0, (_175 + CONST__153) * sizeof(int16_t));
  if ((_164 = new int16_t[_175]) != 0)
    memset(_164, 0, _175 * sizeof(int16_t));
  _165 = new uint8_t[CONST__155];
  _179 = new uint8_t[CONST__156];
  if ((_189 = new uint16_t[2 * CONST__141 - 1]) != 0)
    memset(_189, 0, (2 * CONST__141 - 1) * sizeof(uint16_t));
  if ((_190 = new uint16_t[2 * CONST__141 - 1]) != 0)
    memset(_190, 0, (2 * CONST__141 - 1) * sizeof(uint16_t));
  if ((_177 = new int16_t[CONST__141 + 1]) != 0)
    memset(_177, 0, (CONST__141 + 1) * sizeof(int16_t));
  _180 = new uint8_t[CONST__141];
  if ((_191 = new uint16_t[2 * CONST__141 - 1]) != 0)
    memset(_191, 0, (2 * CONST__141 - 1) * sizeof(uint16_t));
  if ((_192 = new uint16_t[CONST__141]) != 0)
    memset(_192, 0, CONST__141 * sizeof(uint16_t));
  _181 = new uint8_t[CONST__152];
  if ((_193 = new uint16_t[2 * CONST__142 - 1]) != 0)
    memset(_193, 0, (2 * CONST__142 - 1) * sizeof(uint16_t));
  if ((_194 = new uint16_t[CONST__152]) != 0)
    memset(_194, 0, CONST__152 * sizeof(uint16_t));
  if (!_166 || !_163 || !_164 || !_165 || !_179 || !_189 || !_190 || !_177 ||
      !_180 || !_191 || !_192 || !_181 || !_193 || !_194) {
    mStatus.SetError(AL_CANT_ALLOCATE_MEMORY, _520);
  }
  _533 = 0;
  _534 = _266.GetSize();
}
RCompress::~RCompress() {
  if (_166)
    delete[] _166;
  if (_163)
    delete[] _163;
  if (_164)
    delete[] _164;
  if (_165)
    delete[] _165;
  if (_179)
    delete[] _179;
  if (_189)
    delete[] _189;
  if (_190)
    delete[] _190;
  if (_177)
    delete[] _177;
  if (_180)
    delete[] _180;
  if (_191)
    delete[] _191;
  if (_192)
    delete[] _192;
  if (_181)
    delete[] _181;
  if (_193)
    delete[] _193;
  if (_194)
    delete[] _194;
}
void RCompress::fn223(int16_t _203) { fn208(_180[_203], _192[_203]); }
int32_t RCompress::Compress() {
  int16_t local209;
  int16_t local201;
  int16_t local200;
  int16_t s;
  int32_t local231;
  uint8_t *local278;
  int16_t local280;
  int16_t local279;
  local278 = _166;
  local280 = _176;
  local279 = _175;
  local231 = 0;
  fn196();
  fn198();
  local200 = 0;
  local209 = (int16_t)_161->ReadBuffer(local278, local279);
  s = (int16_t)(local209 & local280);
  _169 = 0;
  _168 = 0;
  local201 = (int16_t)(
      ((local278[local200] << CONST__154) ^ (local278[local200 + 1])) &
      (CONST__153 - 1));
  local201 = (int16_t)(MACRO_FN445(local278, local200, local201) + local279);
  while (local209 > CONST__140 + 4 && !_170) {
    fn199(local200, local201);
    if (_168 < CONST__135) {
      fn202(local278[local200], 0);
      MACRO_FN447(_163, _164, local200, local201);
      local200++;
      local201 =
          (int16_t)(MACRO_FN445(local278, local200, local201) + local279);
      local209--;
    } else {
      local209 -= _168;
      fn202((uint16_t)(_168 + (UCHAR_MAX + 1 - CONST__135)), _169);
      while (--_168 >= 0) {
        MACRO_FN447(_163, _164, local200, local201);
        local200++;
        local201 =
            (int16_t)(MACRO_FN445(local278, local200, local201) + local279);
      }
    }
  }
  for (; local209 < CONST__140; local209++) {
    int32_t _203 = _161->ReadChar();
    if (_203 < 0)
      break;
    local278[s] = (unsigned char)_203;
    if (s < CONST__140 - 1)
      local278[s + local279] = local278[s];
    MACRO_FN448(_163, _164, s);
    s = (int16_t)((s + 1) & (local280));
  }
  while (local209 > 0 && !_170) {
    fn199(local200, local201);
    if (_168 > local209)
      _168 = local209;
    if (_168 < CONST__135) {
      _168 = 1;
      fn202(local278[local200], 0);
    } else
      fn202((uint16_t)(_168 + (UCHAR_MAX + 1 - CONST__135)), _169);
    while (--_168 >= 0) {
      int32_t _203 = _161->ReadChar();
      if (_203 < 0)
        break;
      else
        local278[s] = (unsigned char)_203;
      if (s < CONST__140 - 1)
        local278[s + local279] = local278[s];
      MACRO_FN448(_163, _164, s);
      s = (int16_t)((s + 1) & (local280));
      MACRO_FN447(_163, _164, local200, local201);
      local200 = (int16_t)((local200 + 1) & (local280));
      local201 =
          (int16_t)(MACRO_FN445(local278, local200, local201) + local279);
    }
    while (_168-- >= 0) {
      MACRO_FN447(_163, _164, local200, local201);
      local200 = (int16_t)((local200 + 1) & local280);
      local201 =
          (int16_t)(MACRO_FN445(local278, local200, local201) + local279);
      local209--;
    }
    if (_162->mStatus < 0)
      return 1;
  }
  if (!_170)
    fn202(CONST__144 + (UCHAR_MAX + 1 - CONST__135), 0);
  fn197();
  if (_170)
    local231 = 1;
  return local231;
}
void RCompress::fn196() {
  int32_t i;
  for (i = 0; i < CONST__141; i++)
    _191[i] = 0;
  for (i = 0; i < CONST__142; i++)
    _193[i] = 0;
  _173 = 0;
  fn205();
  _170 = 0;
  _185 = 1;
  _184 = 0;
  _186 = 0;
  _165[0] = 0;
  _183 = CONST__155;
  _183 -= (uint16_t)((3 * CHAR_BIT) + 6);
}
void RCompress::fn197() {
  if (!_170)
    fn207();
  fn206();
  _183 = 0;
  _184 = 0;
}
void RCompress::fn198() {
  int16_t *local450;
  int16_t i;
  local450 = &_163[_175];
  for (i = CONST__153; i > 0; i--)
    *local450++ = CONST__157;
  local450 = _164;
  for (i = _175; i > 0; i--)
    *local450++ = CONST__157;
}
void RCompress::fn199(int16_t arg200, int16_t arg201) {
  uint8_t *local451;
  uint8_t *local278;
  int16_t i, local452, local204, local453;
  local452 = CONST__158;
  _168 = 0;
  local451 = &_166[arg200];
  local204 = arg201;
  while ((local204 = _163[local204]) != CONST__157) {
    if (--local452 < 0)
      break;
    local278 = &_166[local204];
    if (local451[_168] != local278[_168])
      continue;
    if (local451[0] != local278[0])
      continue;
    if (local451[1] != local278[1])
      continue;
    if (local451[2] != local278[2])
      continue;
    for (i = 3; i < CONST__140; i++)
      if (local451[i] != local278[i])
        break;
    if (i > _168) {
      local453 = (int16_t)(arg200 - local204 - 1);
      if (local453 < 0)
        local453 += _175;
      if (local453 >= _175) {
        break;
      }
      _169 = local453;
      if ((_168 = i) >= CONST__140)
        break;
    }
  }
}
void RCompress::fn202(uint16_t arg203, uint16_t arg204) {
  if ((_185 >>= 1) == 0) {
    _185 = 1U << (CHAR_BIT - 1);
    if (_184 >= _183) {
      fn207();
      if (_170)
        return;
      _184 = 0;
    }
    _186 = _184++;
    _165[_186] = 0;
  }
  _165[_184++] = (uint8_t)arg203;
  _191[arg203]++;
  if (arg203 >= (1U << CHAR_BIT)) {
    _165[_186] |= (uint8_t)_185;
    _165[_184++] = (uint8_t)arg204;
    _165[_184++] = (uint8_t)(arg204 >> CHAR_BIT);
    arg203 = 0;
    while (arg204) {
      arg203++;
      arg204 >>= 1;
    }
    _193[arg203]++;
  }
}
void RCompress::fn205() {
  _172 = 0;
  _182 = 0;
  _171 = 0;
}
void RCompress::fn206() {
  if (!_170) {
    fn208(CHAR_BIT - 1, 0);
    if (_171)
      fn210();
  }
  _171 = 0;
}
void RCompress::fn207() {
  uint32_t i, local289, local229, local454, local455;
  uint32_t local456 = 0;
  uint16_t local217[2 * CONST__145 - 1];
  local229 = fn211(CONST__141, _191, _180, _192);
  local455 = _191[local229];
  fn208(16, (uint16_t)local455);
  if (local229 >= CONST__141) {
    fn216(local217);
    local229 = fn211(CONST__145, local217, _181, _194);
    if (local229 >= CONST__145) {
      fn218(CONST__145, CONST__147, 3);
    } else {
      fn208(CONST__147, 0);
      fn208(CONST__147, (uint16_t)local229);
    }
    fn222();
  } else {
    fn208(CONST__147, 0);
    fn208(CONST__147, 0);
    fn208(CONST__143, 0);
    fn208(CONST__143, (uint16_t)local229);
  }
  local229 = fn211(CONST__142, _193, _181, _194);
  if (local229 >= CONST__142) {
    fn218(CONST__142, CONST__540, -1);
  } else {
    fn208(CONST__540, 0);
    fn208(CONST__540, (uint16_t)local229);
  }
  local454 = 0;
  for (i = 0; i < local455; i++) {
    if (i % CHAR_BIT == 0)
      local456 = _165[local454++];
    else
      local456 <<= 1;
    if (local456 & (1U << (CHAR_BIT - 1))) {
      fn223((int16_t)(_165[local454++] + (1U << CHAR_BIT)));
      local289 = _165[local454++];
      local289 += _165[local454++] << CHAR_BIT;
      fn224((int16_t)local289);
    } else
      fn223(_165[local454++]);
    if (_170)
      return;
  }
  for (i = 0; i < CONST__141; i++)
    _191[i] = 0;
  for (i = 0; i < CONST__142; i++)
    _193[i] = 0;
}
void RCompress::fn208(int32_t arg209, uint16_t arg203) {
  arg203 <<= CONST__133 - arg209;
  _182 |= (uint16_t)(arg203 >> _172);
  if ((_172 += (int16_t)arg209) >= 8) {
    if (_171 >= CONST__156)
      fn210();
    _179[_171++] = (uint8_t)(_182 >> CHAR_BIT);
    if ((_172 = (uint16_t)(_172 - CHAR_BIT)) < CHAR_BIT)
      _182 <<= CHAR_BIT;
    else {
      if (_171 >= CONST__156)
        fn210();
      _179[_171++] = (uint8_t)_182;
      _172 = (uint16_t)(_172 - CHAR_BIT);
      _182 = (uint16_t)(arg203 << (arg209 - _172));
    }
  }
}
void RCompress::fn210() {
  if (_171 <= 0)
    return;
  if (_531 && (_533 += _171) >= _534)
    _170 = 1;
  else
    _162->WriteBuffer(_179, _171);
  _171 = 0;
}
int32_t RCompress::fn211(int32_t arg212, uint16_t *arg213, uint8_t *arg214,
                         uint16_t *arg215) {
  int32_t i, local276, local289, local292;
  int16_t local227;
  _174 = (int16_t)arg212;
  _187 = arg213;
  _178 = arg214;
  local292 = _174;
  local227 = 0;
  _177[1] = 0;
  for (i = 0; i < _174; i++) {
    _178[i] = 0;
    if (_187[i])
      _177[++local227] = (int16_t)i;
  }
  if (local227 < 2) {
    arg215[_177[1]] = 0;
    return _177[1];
  }
  for (i = local227 / 2; i >= 1; i--)
    fn225(i, _187, _177, local227);
  _188 = arg215;
  do {
    i = _177[1];
    if (i < _174)
      *_188++ = (uint16_t)i;
    _177[1] = _177[local227--];
    fn225(1, _187, _177, local227);
    local276 = _177[1];
    if (local276 < _174)
      *_188++ = (uint16_t)local276;
    local289 = local292++;
    _187[local289] = (uint16_t)(_187[i] + _187[local276]);
    _177[1] = (int16_t)local289;
    fn225(1, _187, _177, local227);
    _189[local289] = (uint16_t)i;
    _190[local289] = (uint16_t)local276;
  } while (local227 > 1);
  _188 = arg215;
  fn228(local289);
  fn230(arg212, arg214, arg215);
  return local289;
}
void RCompress::fn216(uint16_t *arg217) {
  int16_t i, local289, local219, local277;
  for (i = 0; i < CONST__145; i++)
    arg217[i] = 0;
  local219 = CONST__141;
  while (local219 > 0 && _180[local219 - 1] == 0)
    local219--;
  i = 0;
  while (i < local219) {
    local289 = _180[i++];
    if (local289 == 0) {
      local277 = 1;
      while (i < local219 && _180[i] == 0) {
        i++;
        local277++;
      }
      if (local277 <= 2)
        arg217[0] += local277;
      else if (local277 <= 18)
        arg217[1]++;
      else if (local277 == 19) {
        arg217[0]++;
        arg217[1]++;
      } else
        arg217[2]++;
    } else
      arg217[local289 + 2]++;
  }
}
void RCompress::fn218(int16_t arg219, int16_t arg220, int16_t arg221) {
  int16_t i, local289;
  while (arg219 > 0 && _181[arg219 - 1] == 0)
    arg219--;
  fn208(arg220, arg219);
  i = 0;
  while (i < arg219) {
    local289 = _181[i++];
    if (local289 <= 6) {
      fn208(3, local289);
    } else
      fn208(local289 - 3, (uint16_t)(USHRT_MAX << 1));
    if (i == arg221) {
      while (i < 6 && _181[i] == 0)
        i++;
      fn208(2, (uint16_t)(i - 3));
    }
  }
}
void RCompress::fn222() {
  int16_t i, local289, local219, local277;
  local219 = CONST__141;
  while (local219 > 0 && _180[local219 - 1] == 0)
    local219--;
  fn208(CONST__143, local219);
  i = 0;
  while (i < local219) {
    local289 = _180[i++];
    if (local289 == 0) {
      local277 = 1;
      while (i < local219 && _180[i] == 0) {
        i++;
        local277++;
      }
      if (local277 <= 2) {
        for (local289 = 0; local289 < local277; local289++)
          fn208(_181[0], _194[0]);
      } else if (local277 <= 18) {
        fn208(_181[1], _194[1]);
        fn208(4, (uint16_t)(local277 - 3));
      } else if (local277 == 19) {
        fn208(_181[0], _194[0]);
        fn208(_181[1], _194[1]);
        fn208(4, 15);
      } else {
        fn208(_181[2], _194[2]);
        fn208(CONST__143, (uint16_t)(local277 - 20));
      }
    } else
      fn208(_181[local289 + 2], _194[local289 + 2]);
  }
}
void RCompress::fn224(uint16_t arg204) {
  uint16_t local203, local457;
  local203 = 0;
  local457 = arg204;
  while (local457) {
    local203++;
    local457 >>= 1;
  }
  fn208(_181[local203], _194[local203]);
  if (local203 > 1)
    fn208(local203 - 1, arg204);
}
void RCompress::fn225(int32_t i, uint16_t *arg187, int16_t *arg177,
                      int16_t arg227) {
  int32_t local276, local289;
  local289 = arg177[i];
  while ((local276 = 2 * i) <= arg227) {
    if (local276 < arg227 &&
        arg187[arg177[local276]] > arg187[arg177[local276 + 1]])
      local276++;
    if (arg187[local289] <= arg187[arg177[local276]])
      break;
    arg177[i] = arg177[local276];
    i = local276;
  }
  arg177[i] = (uint16_t)local289;
}
void RCompress::fn228(int32_t arg229) {
  int32_t i, local289;
  uint32_t local458;
  for (i = 0; i <= 16; i++)
    _167[i] = 0;
  fn232(arg229);
  local458 = 0;
  for (i = 16; i > 0; i--)
    local458 += _167[i] << (16 - i);
  while (local458 != (1U << 16)) {
    _167[16]--;
    for (i = 15; i > 0; i--) {
      if (_167[i] != 0) {
        _167[i]--;
        _167[i + 1] = (uint16_t)(_167[i + 1] + 2);
        break;
      }
    }
    local458--;
  }
  for (i = 16; i > 0; i--) {
    local289 = _167[i];
    while (--local289 >= 0)
      _178[*_188++] = (uint8_t)i;
  }
}
void RCompress::fn230(int32_t arg219, uint8_t *arg209, uint16_t *arg231) {
  int32_t i;
  uint16_t local288[18];
  local288[1] = 0;
  for (i = 1; i <= 16; i++)
    local288[i + 1] = (uint16_t)((local288[i] + _167[i]) << 1);
  for (i = 0; i < arg219; i++)
    arg231[i] = local288[arg209[i]]++;
}

void RCompress::fn232(int32_t arg226) {
  if (arg226 < _174)
    _167[(_173 < 16) ? _173 : 16]++;
  else {
    _173++;
    fn232(_189[arg226]);
    fn232(_190[arg226]);
    _173--;
  }
}
