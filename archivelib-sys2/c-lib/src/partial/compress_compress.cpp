
#include "r_compress.hpp"

int16_t fn445(uint8_t *arg278, int16_t arg200, int16_t arg446) {
  return ((int16_t)((arg446 << CONST_N154_IS_4) ^ (arg278[arg200 + 2])) &
          (CONST_N153_IS_4096 - 1));
}
void fn447(int16_t *arg163, int16_t *arg164, int16_t arg200, int16_t arg201) {
  int16_t local204;
  if ((local204 = arg163[arg201]) != TRUE157)
    arg164[local204] = arg200;
  arg164[arg200] = arg201;
  arg163[arg200] = local204;
  arg163[arg201] = arg200;
}
void fn448(int16_t *arg163, int16_t *arg164, int16_t s) {
  int16_t local204;
  if ((local204 = arg164[s]) != TRUE157) {
    arg164[s] = TRUE157;
    arg163[local204] = TRUE157;
  }
}

bool RCompress::Compress() {
  int16_t _209;
  int16_t _201;
  int16_t _200;
  int16_t s;
  int _231;
  uint8_t *l278_in_buffer;
  int16_t _280;
  int16_t _279;
  l278_in_buffer = data->input_buffer;
  _280 = data->max_input_data_size_minus_one;
  _279 = data->max_input_data_size;
  _231 = 0;
  reset_compress_data(data);
  fn198();
  _200 = 0;
  _209 = (int16_t)data->input_store->ReadBuffer(l278_in_buffer, _279);
  s = (int16_t)(_209 & _280);
  data->dat169 = 0;
  data->dat168 = 0;
  _201 = (int16_t)(
      ((l278_in_buffer[_200] << CONST_N154_IS_4) ^ (l278_in_buffer[_200 + 1])) &
      (CONST_N153_IS_4096 - 1));
  _201 = (int16_t)(fn445(l278_in_buffer, _200, _201) + _279);
  while (_209 > CONST_N140_IS_256 + 4 && !data->uncompressible) {
    fn199(_200, _201);
    if (data->dat168 < CONST_N135_IS_3) {
      fn202(l278_in_buffer[_200], 0);
      fn447(data->dat_arr163, data->dat_arr164, _200, _201);
      _200++;
      _201 = (int16_t)(fn445(l278_in_buffer, _200, _201) + _279);
      _209--;
    } else {
      _209 -= data->dat168;
      fn202((uint16_t)(data->dat168 + (UCHAR_MAX + 1 - CONST_N135_IS_3)),
            data->dat169);
      while (--data->dat168 >= 0) {
        fn447(data->dat_arr163, data->dat_arr164, _200, _201);
        _200++;
        _201 = (int16_t)(fn445(l278_in_buffer, _200, _201) + _279);
      }
    }
  }
  for (; _209 < CONST_N140_IS_256; _209++) {
    int _203 = data->input_store->ReadChar();
    if (_203 < 0)
      break;
    l278_in_buffer[s] = (unsigned char)_203;
    if (s < CONST_N140_IS_256 - 1)
      l278_in_buffer[s + _279] = l278_in_buffer[s];
    fn448(data->dat_arr163, data->dat_arr164, s);
    s = (int16_t)((s + 1) & (_280));
  }
  while (_209 > 0 && !data->uncompressible) {
    fn199(_200, _201);
    if (data->dat168 > _209)
      data->dat168 = _209;
    if (data->dat168 < CONST_N135_IS_3) {
      data->dat168 = 1;
      fn202(l278_in_buffer[_200], 0);
    } else
      fn202((uint16_t)(data->dat168 + (UCHAR_MAX + 1 - CONST_N135_IS_3)),
            data->dat169);
    while (--data->dat168 >= 0) {
      int _203 = data->input_store->ReadChar();
      if (_203 < 0)
        break;
      else
        l278_in_buffer[s] = (unsigned char)_203;
      if (s < CONST_N140_IS_256 - 1)
        l278_in_buffer[s + _279] = l278_in_buffer[s];
      fn448(data->dat_arr163, data->dat_arr164, s);
      s = (int16_t)((s + 1) & (_280));
      fn447(data->dat_arr163, data->dat_arr164, _200, _201);
      _200 = (int16_t)((_200 + 1) & (_280));
      _201 = (int16_t)(fn445(l278_in_buffer, _200, _201) + _279);
    }
    while (data->dat168-- >= 0) {
      fn447(data->dat_arr163, data->dat_arr164, _200, _201);
      _200 = (int16_t)((_200 + 1) & _280);
      _201 = (int16_t)(fn445(l278_in_buffer, _200, _201) + _279);
      _209--;
    }
    if (data->output_store->mStatus < 0)
      return 1;
  }
  if (!data->uncompressible)
    fn202(CONST_N144_IS_257 + (UCHAR_MAX + 1 - CONST_N135_IS_3), 0);
  finalise_compresson197();
  if (data->uncompressible)
    _231 = 1;
  return _231;
}
