
#include "r_compress.hpp"

int32_t RCompress::fn211(int32_t _212, uint16_t *_213, uint8_t *_214, uint16_t *_215) {
  int32_t run_start226, run_length276, _289, _292;
  int16_t _227;
  data->dat174 = (int16_t)_212;
  data->dat_arr_cursor187 = _213;
  data->dat_arr_cursor178 = _214;
  _292 = data->dat174;
  _227 = 0;
  data->dat_arr177[1] = 0;
  for (run_start226 = 0; run_start226 < data->dat174; run_start226++) {
    data->dat_arr_cursor178[run_start226] = 0;
    if (data->dat_arr_cursor187[run_start226])
      data->dat_arr177[++_227] = (int16_t)run_start226;
  }
  if (_227 < 2) {
    _215[data->dat_arr177[1]] = 0;
    return data->dat_arr177[1];
  }
  for (run_start226 = _227 / 2; run_start226 >= 1; run_start226--)
    fn225(run_start226, data->dat_arr_cursor187, data->dat_arr177, _227);
  data->dat_arr_cursor188 = _215;
  do {
    run_start226 = data->dat_arr177[1];
    if (run_start226 < data->dat174)
      *data->dat_arr_cursor188++ = (uint16_t)run_start226;
    data->dat_arr177[1] = data->dat_arr177[_227--];
    fn225(1, data->dat_arr_cursor187, data->dat_arr177, _227);
    run_length276 = data->dat_arr177[1];
    if (run_length276 < data->dat174)
      *data->dat_arr_cursor188++ = (uint16_t)run_length276;
    _289 = _292++;
    data->dat_arr_cursor187[_289] = (uint16_t)(data->dat_arr_cursor187[run_start226] +
                                               data->dat_arr_cursor187[run_length276]);
    data->dat_arr177[1] = (int16_t)_289;
    fn225(1, data->dat_arr_cursor187, data->dat_arr177, _227);
    data->dat_arr189[_289] = (uint16_t)run_start226;
    data->dat_arr190[_289] = (uint16_t)run_length276;
  } while (_227 > 1);
  data->dat_arr_cursor188 = _215;
  fn228(_289);
  fn230(_212, _214, _215);
  return _289;
}
