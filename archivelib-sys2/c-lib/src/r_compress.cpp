
#include "r_compress.hpp"

inline void RCompress::write_stored_bits_to_buffer(int16_t _203) {
  write_bits_to_buffer(data->dat_arr180[_203], data->dat_arr192[_203]);
}

void RCompress::finalise_compresson197() {
  if (!data->uncompressible)
    fn207();
  finalize_buffer206();
  data->dat183_IS_CONST_8162 = 0;
  data->array165_counter = 0;
}
void RCompress::fn198() {
  int16_t *_450;
  int16_t _226;
  _450 = &data->dat_arr163[data->max_input_data_size];
  for (_226 = CONST_N153_IS_4096; _226 > 0; _226--)
    *_450++ = TRUE157;
  _450 = data->dat_arr164;
  for (_226 = data->max_input_data_size; _226 > 0; _226--)
    *_450++ = TRUE157;
}
void RCompress::fn199(int16_t input_buffer_index200, int16_t _201) {
  uint8_t *_451;
  uint8_t *_278;
  int16_t _226, _452, _204, _453;
  _452 = MAX_COMPRESSION_CYCLES;
  data->dat168 = 0;
  _451 = &data->input_buffer[input_buffer_index200];
  _204 = _201;
  while ((_204 = data->dat_arr163[_204]) != TRUE157) {
    if (--_452 < 0)
      break;
    _278 = &data->input_buffer[_204];
    if (_451[data->dat168] != _278[data->dat168])
      continue;
    if (_451[0] != _278[0])
      continue;
    if (_451[1] != _278[1])
      continue;
    if (_451[2] != _278[2])
      continue;
    for (_226 = 3; _226 < CONST_N140_IS_256; _226++)
      if (_451[_226] != _278[_226])
        break;
    if (_226 > data->dat168) {
      _453 = (int16_t)(input_buffer_index200 - _204 - 1);
      if (_453 < 0)
        _453 += data->max_input_data_size;
      if (_453 >= data->max_input_data_size) {
        break;
      }
      data->dat169 = _453;
      if ((data->dat168 = _226) >= CONST_N140_IS_256)
        break;
    }
  }
}
void RCompress::fn202(uint16_t _203, uint16_t _204) {
  if ((data->bitwise_counter185 >>= 1) == 0) {
    data->bitwise_counter185 = 1U << (CHAR_BIT - 1);
    if (data->array165_counter >= data->dat183_IS_CONST_8162) {
      fn207();
      if (data->uncompressible)
        return;
      data->array165_counter = 0;
    }
    data->array165_tmp_counter186 = data->array165_counter++;
    data->dat_arr165[data->array165_tmp_counter186] = 0;
  }
  data->dat_arr165[data->array165_counter++] = (uint8_t)_203;
  data->dat_arr191[_203]++;
  if (_203 >= (1U << CHAR_BIT)) {
    data->dat_arr165[data->array165_tmp_counter186] |=
        (uint8_t)data->bitwise_counter185;
    data->dat_arr165[data->array165_counter++] = (uint8_t)_204;
    data->dat_arr165[data->array165_counter++] = (uint8_t)(_204 >> CHAR_BIT);
    _203 = 0;
    while (_204) {
      _203++;
      _204 >>= 1;
    }
    data->dat_arr193[_203]++;
  }
}
void RCompress::fn207() {
  uint32_t _226, _289, _229, _454, _455;
  uint32_t _456 = 0;
  uint16_t _217[2 * CONST_N145_IS_19 - 1];
  _229 = fn211(CONST_N141_IS_511, data->dat_arr191, data->dat_arr180,
              data->dat_arr192);
  _455 = data->dat_arr191[_229];
  write_bits_to_buffer(16, (uint16_t)_455);
  if (_229 >= CONST_N141_IS_511) {
    fn216(_217);
    _229 = fn211(CONST_N145_IS_19, _217, data->dat_arr181, data->dat_arr194);
    if (_229 >= CONST_N145_IS_19) {
      fn218(CONST_N145_IS_19, CONST_N147_IS_5, 3);
    } else {
      write_bits_to_buffer(CONST_N147_IS_5, 0);
      write_bits_to_buffer(CONST_N147_IS_5, (uint16_t)_229);
    }
    fn222();
  } else {
    write_bits_to_buffer(CONST_N147_IS_5, 0);
    write_bits_to_buffer(CONST_N147_IS_5, 0);
    write_bits_to_buffer(CONST_N143_IS_9, 0);
    write_bits_to_buffer(CONST_N143_IS_9, (uint16_t)_229);
  }
  _229 = fn211(CONST_N142_IS_15, data->dat_arr193, data->dat_arr181,
              data->dat_arr194);
  if (_229 >= CONST_N142_IS_15) {
    fn218(CONST_N142_IS_15, CONST_N540_IS_5, -1);
  } else {
    write_bits_to_buffer(CONST_N540_IS_5, 0);
    write_bits_to_buffer(CONST_N540_IS_5, (uint16_t)_229);
  }
  _454 = 0;
  for (_226 = 0; _226 < _455; _226++) {
    if (_226 % CHAR_BIT == 0)
      _456 = data->dat_arr165[_454++];
    else
      _456 <<= 1;
    if (_456 & (1U << (CHAR_BIT - 1))) {
      write_stored_bits_to_buffer(
          (int16_t)(data->dat_arr165[_454++] + (1U << CHAR_BIT)));
      _289 = data->dat_arr165[_454++];
      _289 += data->dat_arr165[_454++] << CHAR_BIT;
      fn224((int16_t)_289);
    } else
      write_stored_bits_to_buffer(data->dat_arr165[_454++]);
    if (data->uncompressible)
      return;
  }
  for (_226 = 0; _226 < CONST_N141_IS_511; _226++)
    data->dat_arr191[_226] = 0;
  for (_226 = 0; _226 < CONST_N142_IS_15; _226++)
    data->dat_arr193[_226] = 0;
}
void RCompress::write_bits_to_buffer(int _209, uint16_t _203) {
  _203 <<= UINT16_BIT - _209;
  data->bits_buffer182 |= (uint16_t)(_203 >> data->bits_buffer182);
  if ((data->bits_buffer182 += (int16_t)_209) >= 8) {
    if (data->buffer_position >= BUFFER_SIZE)
      flush_to_output();
    data->buffer[data->buffer_position++] = (uint8_t)(data->bits_buffer182 >> CHAR_BIT);
    if ((data->bits_buffer182 = (uint16_t)(data->bits_buffer182 - CHAR_BIT)) < CHAR_BIT)
      data->bits_buffer182 <<= CHAR_BIT;
    else {
      if (data->buffer_position >= BUFFER_SIZE)
        flush_to_output();
      data->buffer[data->buffer_position++] = (uint8_t)data->bits_buffer182;
      data->bits_buffer182 = (uint16_t)(data->bits_buffer182 - CHAR_BIT);
      data->bits_buffer182 = (uint16_t)(_203 << (_209 - data->bits_buffer182));
    }
  }
}
void RCompress::flush_to_output() {
  if (data->buffer_position <= 0)
    return;
  if (_531 && (_533 += data->buffer_position) >= _534)
    data->uncompressible = 1;
  else
    data->output_store->WriteBuffer(data->buffer, data->buffer_position);
  data->buffer_position = 0;
}
int RCompress::fn211(int _212, uint16_t *_213, uint8_t *_214, uint16_t *_215) {
  int _226, _276, _289, _292;
  int16_t _227;
  data->dat174 = (int16_t)_212;
  data->dat_arr_cursor187 = _213;
  data->dat_arr_cursor178 = _214;
  _292 = data->dat174;
  _227 = 0;
  data->dat_arr177[1] = 0;
  for (_226 = 0; _226 < data->dat174; _226++) {
    data->dat_arr_cursor178[_226] = 0;
    if (data->dat_arr_cursor187[_226])
      data->dat_arr177[++_227] = (int16_t)_226;
  }
  if (_227 < 2) {
    _215[data->dat_arr177[1]] = 0;
    return data->dat_arr177[1];
  }
  for (_226 = _227 / 2; _226 >= 1; _226--)
    fn225(_226, data->dat_arr_cursor187, data->dat_arr177, _227);
  data->dat_arr_cursor188 = _215;
  do {
    _226 = data->dat_arr177[1];
    if (_226 < data->dat174)
      *data->dat_arr_cursor188++ = (uint16_t)_226;
    data->dat_arr177[1] = data->dat_arr177[_227--];
    fn225(1, data->dat_arr_cursor187, data->dat_arr177, _227);
    _276 = data->dat_arr177[1];
    if (_276 < data->dat174)
      *data->dat_arr_cursor188++ = (uint16_t)_276;
    _289 = _292++;
    data->dat_arr_cursor187[_289] = (uint16_t)(data->dat_arr_cursor187[_226] +
                                               data->dat_arr_cursor187[_276]);
    data->dat_arr177[1] = (int16_t)_289;
    fn225(1, data->dat_arr_cursor187, data->dat_arr177, _227);
    data->dat_arr189[_289] = (uint16_t)_226;
    data->dat_arr190[_289] = (uint16_t)_276;
  } while (_227 > 1);
  data->dat_arr_cursor188 = _215;
  fn228(_289);
  fn230(_212, _214, _215);
  return _289;
}
void RCompress::fn216(uint16_t *_217) {
  int16_t _226, _289, _219, _277;
  for (_226 = 0; _226 < CONST_N145_IS_19; _226++)
    _217[_226] = 0;
  _219 = CONST_N141_IS_511;
  while (_219 > 0 && data->dat_arr180[_219 - 1] == 0)
    _219--;
  _226 = 0;
  while (_226 < _219) {
    _289 = data->dat_arr180[_226++];
    if (_289 == 0) {
      _277 = 1;
      while (_226 < _219 && data->dat_arr180[_226] == 0) {
        _226++;
        _277++;
      }
      if (_277 <= 2)
        _217[0] += _277;
      else if (_277 <= 18)
        _217[1]++;
      else if (_277 == 19) {
        _217[0]++;
        _217[1]++;
      } else
        _217[2]++;
    } else
      _217[_289 + 2]++;
  }
}
void RCompress::fn218(int16_t _219, int16_t _220, int16_t _221) {
  int16_t _226, _289;
  while (_219 > 0 && data->dat_arr181[_219 - 1] == 0)
    _219--;
  write_bits_to_buffer(_220, _219);
  _226 = 0;
  while (_226 < _219) {
    _289 = data->dat_arr181[_226++];
    if (_289 <= 6) {
      write_bits_to_buffer(3, _289);
    } else
      write_bits_to_buffer(_289 - 3, (uint16_t)(USHRT_MAX << 1));
    if (_226 == _221) {
      while (_226 < 6 && data->dat_arr181[_226] == 0)
        _226++;
      write_bits_to_buffer(2, (uint16_t)(_226 - 3));
    }
  }
}
void RCompress::fn222() {
  int16_t _226, _289, _219, _277;
  _219 = CONST_N141_IS_511;
  while (_219 > 0 && data->dat_arr180[_219 - 1] == 0)
    _219--;
  write_bits_to_buffer(CONST_N143_IS_9, _219);
  _226 = 0;
  while (_226 < _219) {
    _289 = data->dat_arr180[_226++];
    if (_289 == 0) {
      _277 = 1;
      while (_226 < _219 && data->dat_arr180[_226] == 0) {
        _226++;
        _277++;
      }
      if (_277 <= 2) {
        for (_289 = 0; _289 < _277; _289++)
          write_bits_to_buffer(data->dat_arr181[0], data->dat_arr194[0]);
      } else if (_277 <= 18) {
        write_bits_to_buffer(data->dat_arr181[1], data->dat_arr194[1]);
        write_bits_to_buffer(4, (uint16_t)(_277 - 3));
      } else if (_277 == 19) {
        write_bits_to_buffer(data->dat_arr181[0], data->dat_arr194[0]);
        write_bits_to_buffer(data->dat_arr181[1], data->dat_arr194[1]);
        write_bits_to_buffer(4, 15);
      } else {
        write_bits_to_buffer(data->dat_arr181[2], data->dat_arr194[2]);
        write_bits_to_buffer(CONST_N143_IS_9, (uint16_t)(_277 - 20));
      }
    } else
      write_bits_to_buffer(data->dat_arr181[_289 + 2],
                           data->dat_arr194[_289 + 2]);
  }
}
void RCompress::fn224(uint16_t _204) {
  uint16_t _203, _457;
  _203 = 0;
  _457 = _204;
  while (_457) {
    _203++;
    _457 >>= 1;
  }
  write_bits_to_buffer(data->dat_arr181[_203], data->dat_arr194[_203]);
  if (_203 > 1)
    write_bits_to_buffer(_203 - 1, _204);
}
void RCompress::fn225(int _226, uint16_t * data->dat_arr_cursor187,
                     int16_t * data->dat_arr177, int16_t _227) {
  int _276, _289;
  _289 = data->dat_arr177[_226];
  while ((_276 = 2 * _226) <= _227) {
    if (_276 < _227 && data->dat_arr_cursor187[data->dat_arr177[_276]] >
                           data->dat_arr_cursor187[data->dat_arr177[_276 + 1]])
      _276++;
    if (data->dat_arr_cursor187[_289] <=
        data->dat_arr_cursor187[data->dat_arr177[_276]])
      break;
    data->dat_arr177[_226] = data->dat_arr177[_276];
    _226 = _276;
  }
  data->dat_arr177[_226] = (uint16_t)_289;
}
void RCompress::fn228(int _229) {
  int _226, _289;
  uint32_t _458;
  for (_226 = 0; _226 <= 16; _226++)
    data->dat_arr167[_226] = 0;
  fn232(_229);
  _458 = 0;
  for (_226 = 16; _226 > 0; _226--)
    _458 += data->dat_arr167[_226] << (16 - _226);
  while (_458 != (1U << 16)) {
    data->dat_arr167[16]--;
    for (_226 = 15; _226 > 0; _226--) {
      if (data->dat_arr167[_226] != 0) {
        data->dat_arr167[_226]--;
        data->dat_arr167[_226 + 1] = (uint16_t)(data->dat_arr167[_226 + 1] + 2);
        break;
      }
    }
    _458--;
  }
  for (_226 = 16; _226 > 0; _226--) {
    _289 = data->dat_arr167[_226];
    while (--_289 >= 0)
      data->dat_arr_cursor178[*data->dat_arr_cursor188++] = (uint8_t)_226;
  }
}
void RCompress::fn230(int _219, uint8_t *_209, uint16_t *_231) {
  int _226;
  uint16_t _288[18];
  _288[1] = 0;
  for (_226 = 1; _226 <= 16; _226++)
    _288[_226 + 1] = (uint16_t)((_288[_226] + data->dat_arr167[_226]) << 1);
  for (_226 = 0; _226 < _219; _226++)
    _231[_226] = _288[_209[_226]]++;
}
void RCompress::fn232(int _226) {
  if (_226 < data->dat174)
    data->dat_arr167[(data->dat173 < 16) ? data->dat173 : 16]++;
  else {
    data->dat173++;
    fn232(data->dat_arr189[_226]);
    fn232(data->dat_arr190[_226]);
    data->dat173--;
  }
}
