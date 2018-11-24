
#include "r_compress.hpp"

void fn207(RCompressData *data) {
  uint32_t run_start226, _289, _229, _454, _455;
  uint32_t _456 = 0;
  uint16_t _217[2 * CONST_N145_IS_19 - 1];
  _229 = fn211(data, CONST_N141_IS_511, data->dat_arr191, data->dat_arr180,
               data->dat_arr192);
  _455 = data->dat_arr191[_229];
  write_bits_to_buffer(data, 16, (uint16_t)_455);
  if (_229 >= CONST_N141_IS_511) {
    fn216(data, _217);
    _229 =
        fn211(data, CONST_N145_IS_19, _217, data->dat_arr181, data->dat_arr194);
    if (_229 >= CONST_N145_IS_19) {
      fn218(data, CONST_N145_IS_19, CONST_N147_IS_5, 3);
    } else {
      write_bits_to_buffer(data, CONST_N147_IS_5, 0);
      write_bits_to_buffer(data, CONST_N147_IS_5, (uint16_t)_229);
    }
    fn222(data);
  } else {
    write_bits_to_buffer(data, CONST_N147_IS_5, 0);
    write_bits_to_buffer(data, CONST_N147_IS_5, 0);
    write_bits_to_buffer(data, CONST_N143_IS_9, 0);
    write_bits_to_buffer(data, CONST_N143_IS_9, (uint16_t)_229);
  }
  _229 = fn211(data, CONST_N142_IS_15, data->dat_arr193, data->dat_arr181,
               data->dat_arr194);
  if (_229 >= CONST_N142_IS_15) {
    fn218(data, CONST_N142_IS_15, CONST_N540_IS_5, -1);
  } else {
    write_bits_to_buffer(data, CONST_N540_IS_5, 0);
    write_bits_to_buffer(data, CONST_N540_IS_5, (uint16_t)_229);
  }
  _454 = 0;
  for (run_start226 = 0; run_start226 < _455; run_start226++) {
    if (run_start226 % CHAR_BIT == 0)
      _456 = data->dat_arr165[_454++];
    else
      _456 <<= 1;
    if (_456 & (1U << (CHAR_BIT - 1))) {
      write_stored_bits_to_buffer(
          data, (int16_t)(data->dat_arr165[_454++] + (1U << CHAR_BIT)));
      _289 = data->dat_arr165[_454++];
      _289 += data->dat_arr165[_454++] << CHAR_BIT;
      fn224(data, (int16_t)_289);
    } else
      write_stored_bits_to_buffer(data, data->dat_arr165[_454++]);
    if (data->uncompressible)
      return;
  }
  for (run_start226 = 0; run_start226 < CONST_N141_IS_511; run_start226++)
    data->dat_arr191[run_start226] = 0;
  for (run_start226 = 0; run_start226 < CONST_N142_IS_15; run_start226++)
    data->dat_arr193[run_start226] = 0;
}
