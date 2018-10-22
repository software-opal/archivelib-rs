#include <cstring>

#include "r_expand.hpp"

RExpand::RExpand(ALStorage &_266, ALStorage &_267, size_t _268, int32_t _269) {
  data->input_store = &_266;
  data->output_store = &_267;
  data->dat248 = _268;

  if (_269 > MAX_COMPRESSION_FACTOR || _269 < MIN_COMPRESSION_FACTOR) {
    mStatus.SetError(AL_ILLEGAL_PARAMETER, INVALID_COMPRESSION_LEVEL_MSG,
                     _269 - 10);
    data->max_input_data_size = 2;
  } else
    data->max_input_data_size = (int16_t)(1 << _269);
  data->max_input_data_size_minus_one =
      (int16_t)(data->max_input_data_size - 1);
  data->input_buffer = new uint8_t[data->max_input_data_size + 2];
  if (data->input_buffer)
    memset(data->input_buffer, 0,
           (data->max_input_data_size + 2) * sizeof(uint8_t));
  data->dat_arr240 = new uint16_t[CONST_N148_IS_4096];
  if (data->dat_arr240)
    memset(data->dat_arr240, 0, CONST_N148_IS_4096 * sizeof(uint16_t));
  data->dat_arr241 = new uint16_t[CONST_N149_IS_256];
  if (data->dat_arr241)
    memset(data->dat_arr241, 0, CONST_N149_IS_256 * sizeof(uint16_t));
  data->dat_arr242 = new uint8_t[BUFFER_SIZE];
  if (data->dat_arr242)
    memset(data->dat_arr242, 0, BUFFER_SIZE * sizeof(uint8_t));
  data->dat_arr189 = new uint16_t[2 * CONST_N141_IS_511 - 1];
  if (data->dat_arr189)
    memset(data->dat_arr189, 0, (2 * CONST_N141_IS_511 - 1) * sizeof(uint16_t));
  data->dat_arr190 = new uint16_t[2 * CONST_N141_IS_511 - 1];
  if (data->dat_arr190)
    memset(data->dat_arr190, 0, (2 * CONST_N141_IS_511 - 1) * sizeof(uint16_t));
  data->dat_arr180 = new uint8_t[CONST_N141_IS_511];
  data->dat_arr181 = new uint8_t[CONST_N152_IS_19];
  if (!data->input_buffer || !data->dat_arr240 || !data->dat_arr241 ||
      !data->dat_arr242 || !data->dat_arr189 || !data->dat_arr190 ||
      !data->dat_arr180 || !data->dat_arr181) {
    mStatus.SetError(AL_CANT_ALLOCATE_MEMORY, MEMORY_ALLOCATION_FAILURE_MSG);
  }
}
RExpand::~RExpand() {
  if (data->input_buffer)
    delete[] data->input_buffer;
  if (data->dat_arr240)
    delete[] data->dat_arr240;
  if (data->dat_arr241)
    delete[] data->dat_arr241;
  if (data->dat_arr242)
    delete[] data->dat_arr242;
  if (data->dat_arr189)
    delete[] data->dat_arr189;
  if (data->dat_arr190)
    delete[] data->dat_arr190;
  if (data->dat_arr180)
    delete[] data->dat_arr180;
  if (data->dat_arr181)
    delete[] data->dat_arr181;
}
int32_t RExpand::Expand() {
  int32_t _231;
  int16_t i;
  int16_t _276;
  int16_t _203;
  int16_t input_buffer_index200;
  uint8_t *_278;
  int16_t _279;
  int16_t _280;
  _278 = data->input_buffer;
  _279 = data->max_input_data_size;
  _280 = data->max_input_data_size_minus_one;
  _231 = 0;
  data->dat243 = 0;
  fn251();
  input_buffer_index200 = 0;
  while (data->dat243 < 5) {
    if ((_203 = fn249()) <= UCHAR_MAX) {
      _278[input_buffer_index200] = (uint8_t)_203;
      if (++input_buffer_index200 >= _279) {
        input_buffer_index200 = 0;
        if ((int16_t)data->output_store->WriteBuffer(_278, _279) != _279)
          goto _282;
      }
    } else {
      _276 = (int16_t)(_203 - (UCHAR_MAX + 1 - CONST_N135_IS_3));
      if (_276 == CONST_N144_IS_257)
        break;
      i = (int16_t)((input_buffer_index200 - fn250() - 1) & _280);
      if (i < _279 - CONST_N140_IS_256 - 1 &&
          input_buffer_index200 < _279 - CONST_N140_IS_256 - 1) {
        while (--_276 >= 0)
          _278[input_buffer_index200++] = _278[i++];
      } else {
        while (--_276 >= 0) {
          _278[input_buffer_index200] = _278[i];
          if (++input_buffer_index200 >= _279) {
            input_buffer_index200 = 0;
            if ((int16_t)data->output_store->WriteBuffer(_278, _279) != _279)
              goto _282;
          }
          i = (int16_t)((i + 1) & _280);
        }
      }
    }
  }
  if (input_buffer_index200 != 0)
    data->output_store->WriteBuffer(_278, input_buffer_index200);
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
  _276 = data->dat_arr240[data->bits_buffer182 >> 4];
  if (_276 >= CONST_N141_IS_511) {
    _283 = 1U << 3;
    do {
      if (data->bits_buffer182 & _283)
        _276 = data->dat_arr190[_276];
      else
        _276 = data->dat_arr189[_276];
      _283 >>= 1;
    } while (_276 >= CONST_N141_IS_511);
  }
  fn256(data->dat_arr180[_276]);
  return _276;
}
uint16_t RExpand::fn250() {
  uint16_t _276, _283;
  _276 = data->dat_arr241[data->bits_buffer182 >> 8];
  if (_276 >= CONST_N142_IS_15) {
    _283 = 1U << 7;
    do {
      if (data->bits_buffer182 & _283)
        _276 = data->dat_arr190[_276];
      else
        _276 = data->dat_arr189[_276];
      _283 >>= 1;
    } while (_276 >= CONST_N142_IS_15);
  }
  fn256(data->dat_arr181[_276]);
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
uint16_t RExpand::fn252(int32_t _219) {
  uint16_t _284;
  _284 = (uint16_t)(data->bits_buffer182 >> (2 * CHAR_BIT - _219));
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
      data->dat_arr181[i] = 0;
    for (i = 0; i < 256; i++)
      data->dat_arr241[i] = _203;
  } else {
    i = 0;
    while (i < _219) {
      _203 = (int16_t)(data->bits_buffer182 >> 13);
      if (_203 == 7) {
        _283 = 1U << 12;
        while (_283 & data->bits_buffer182) {
          _283 >>= 1;
          _203++;
        }
      }
      fn256((_203 < 7) ? 3 : _203 - 3);
      data->dat_arr181[i++] = (uint8_t)_203;
      if (i == _221) {
        _203 = fn252(2);
        while (--_203 >= 0)
          data->dat_arr181[i++] = 0;
      }
    }
    while (i < _254)
      data->dat_arr181[i++] = 0;
    fn258(_254, data->dat_arr181, 8, data->dat_arr241, CONST_N149_IS_256);
  }
}
void RExpand::fn255() {
  int16_t i, _203, _219;
  uint16_t _283;
  _219 = fn252(CONST_N143_IS_9);
  if (_219 == 0) {
    _203 = fn252(CONST_N143_IS_9);
    for (i = 0; i < CONST_N141_IS_511; i++)
      data->dat_arr180[i] = 0;
    for (i = 0; i < CONST_N148_IS_4096; i++)
      data->dat_arr240[i] = _203;
  } else {
    i = 0;
    while (i < _219) {
      _203 = data->dat_arr241[data->bits_buffer182 >> 8];
      if (_203 >= CONST_N145_IS_19) {
        _283 = 1U << 7;
        do {
          if (data->bits_buffer182 & _283)
            _203 = data->dat_arr190[_203];
          else
            _203 = data->dat_arr189[_203];
          _283 >>= 1;
        } while (_203 >= CONST_N145_IS_19);
      }
      fn256(data->dat_arr181[_203]);
      if (_203 <= 2) {
        if (_203 == 0)
          _203 = 1;
        else if (_203 == 1)
          _203 = (int16_t)(fn252(4) + 3);
        else
          _203 = (int16_t)(fn252(CONST_N143_IS_9) + 20);
        while (--_203 >= 0)
          data->dat_arr180[i++] = 0;
      } else
        data->dat_arr180[i++] = (uint8_t)(_203 - 2);
    }
    while (i < CONST_N141_IS_511)
      data->dat_arr180[i++] = 0;
    fn258(CONST_N141_IS_511, data->dat_arr180, 12, data->dat_arr240,
          CONST_N148_IS_4096);
  }
}
void RExpand::fn256(int32_t _219) {
  while (_219 > data->bits_buffer_used172) {
    _219 -= data->bits_buffer_used172;
    data->bits_buffer182 =
        (uint16_t)((data->bits_buffer182 << data->bits_buffer_used172) +
                   (data->dat245 >> (CHAR_BIT - data->bits_buffer_used172)));
    if (data->dat246 <= 0) {
      data->dat_arr247 = data->dat_arr242;
      if (data->dat248 >= 0 && data->dat248 < BUFFER_SIZE) {
        data->dat246 = (int16_t)data->input_store->ReadBuffer(
            data->dat_arr242, (size_t)data->dat248);
        data->dat248 -= data->dat246;
      } else
        data->dat246 = (int16_t)data->input_store->ReadBuffer(data->dat_arr242,
                                                              BUFFER_SIZE);
      if (data->dat246 <= 0)
        data->dat243++;
    }
    data->dat245 = *data->dat_arr247++;
    data->dat246--;
    data->bits_buffer_used172 = CHAR_BIT;
  }
  data->bits_buffer_used172 = (int16_t)(data->bits_buffer182 - _219);
  data->bits_buffer182 = (uint16_t)((data->bits_buffer182 << _219) +
                                    (data->dat245 >> (CHAR_BIT - _219)));
  data->dat245 <<= _219;
}
void RExpand::fn257() {
  data->bits_buffer_used172 = 0;
  data->dat245 = 0;
  data->bits_buffer182 = 0;
  data->dat246 = 0;
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
    mStatus.SetError(AL_INTERNAL_ERROR, INTERNAL_ERROR_1_MSG);
    data->dat243 = 10;
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
        mStatus.SetError(AL_INTERNAL_ERROR, INTERNAL_ERROR_2_MSG);
        data->dat243 = 10;
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
          data->dat_arr190[_292] = data->dat_arr189[_292] = 0;
          *_204 = (uint16_t)_292++;
        }
        if (_289 & _283)
          _204 = &data->dat_arr190[*_204];
        else
          _204 = &data->dat_arr189[*_204];
        _289 <<= 1;
        i--;
      }
      *_204 = (uint16_t)_290;
    }
    _288[_209] = (uint16_t)_293;
  }
}
