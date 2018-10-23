
#include "r_compress.hpp"

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
