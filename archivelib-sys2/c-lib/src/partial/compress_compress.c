
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

bool Compress(RCompressData *data) {
  int16_t _209;
  int16_t _201;
  int16_t buffer_pos;
  int16_t s;
  int _231;
  uint8_t *l_uncompressed_buffer278;
  int16_t size_bitmask280;
  int16_t max_size279;
  l_uncompressed_buffer278 = data->uncompressed_buffer;
  size_bitmask280 = data->max_uncompressed_data_size_bitmask;
  max_size279 = data->max_uncompressed_data_size;
  _231 = 0;
  reset_compress_data(data);
  buffer_pos = 0;
  _209 = (int16_t)ALStorage_ReadBuffer(data->input_store,
                                       l_uncompressed_buffer278, max_size279);
  s = (int16_t)(_209 & size_bitmask280);
  data->dat169 = 0;
  data->dat168 = 0;
  _201 = (int16_t)(((l_uncompressed_buffer278[buffer_pos] << CONST_N154_IS_4) ^
                    (l_uncompressed_buffer278[buffer_pos + 1])) &
                   (CONST_N153_IS_4096 - 1));
  printf("%x %x -> %x\n", l_uncompressed_buffer278[buffer_pos], l_uncompressed_buffer278[buffer_pos +1], _201);
  _201 = (int16_t)(fn445(l_uncompressed_buffer278, buffer_pos, _201) +
                   max_size279);
  while (_209 > MAX_RUN_LENGTH140 + 4 && !data->uncompressible) {
    fn199(data, buffer_pos, _201);
    if (data->dat168 < MIN_RUN_LENGTH135_IS_3) {
      fn202(data, l_uncompressed_buffer278[buffer_pos], 0);
      fn447(data->dat_arr163, data->dat_arr164, buffer_pos, _201);
      buffer_pos++;
      _201 = (int16_t)(fn445(l_uncompressed_buffer278, buffer_pos, _201) +
                       max_size279);
      _209--;
    } else {
      _209 -= data->dat168;
      fn202(data,
            (uint16_t)(data->dat168 + (UCHAR_MAX + 1 - MIN_RUN_LENGTH135_IS_3)),
            data->dat169);
      while (--data->dat168 >= 0) {
        fn447(data->dat_arr163, data->dat_arr164, buffer_pos, _201);
        buffer_pos++;
        _201 = (int16_t)(fn445(l_uncompressed_buffer278, buffer_pos, _201) +
                         max_size279);
      }
    }
  }
  for (; _209 < MAX_RUN_LENGTH140; _209++) {
    int byte_or_run_length203 = ALStorage_ReadChar(data->input_store);
    if (byte_or_run_length203 < 0)
      break;
    l_uncompressed_buffer278[s] = (unsigned char)byte_or_run_length203;
    if (s < MAX_RUN_LENGTH140 - 1)
      l_uncompressed_buffer278[s + max_size279] = l_uncompressed_buffer278[s];
    fn448(data->dat_arr163, data->dat_arr164, s);
    s = (int16_t)((s + 1) & (size_bitmask280));
  }
  while (_209 > 0 && !data->uncompressible) {
    fn199(data, buffer_pos, _201);
    if (data->dat168 > _209)
      data->dat168 = _209;
    if (data->dat168 < MIN_RUN_LENGTH135_IS_3) {
      data->dat168 = 1;
      fn202(data, l_uncompressed_buffer278[buffer_pos], 0);
    } else
      fn202(data,
            (uint16_t)(data->dat168 + (UCHAR_MAX + 1 - MIN_RUN_LENGTH135_IS_3)),
            data->dat169);
    while (--data->dat168 >= 0) {
      int byte_or_run_length203 = ALStorage_ReadChar(data->input_store);
      if (byte_or_run_length203 < 0)
        break;
      else
        l_uncompressed_buffer278[s] = (unsigned char)byte_or_run_length203;
      if (s < MAX_RUN_LENGTH140 - 1)
        l_uncompressed_buffer278[s + max_size279] = l_uncompressed_buffer278[s];
      fn448(data->dat_arr163, data->dat_arr164, s);
      s = (int16_t)((s + 1) & (size_bitmask280));
      fn447(data->dat_arr163, data->dat_arr164, buffer_pos, _201);
      buffer_pos = (int16_t)((buffer_pos + 1) & (size_bitmask280));
      _201 = (int16_t)(fn445(l_uncompressed_buffer278, buffer_pos, _201) +
                       max_size279);
    }
    while (data->dat168-- >= 0) {
      fn447(data->dat_arr163, data->dat_arr164, buffer_pos, _201);
      buffer_pos = (int16_t)((buffer_pos + 1) & size_bitmask280);
      _201 = (int16_t)(fn445(l_uncompressed_buffer278, buffer_pos, _201) +
                       max_size279);
      _209--;
    }
    if (ALStorage_mStatus(data->output_store) < 0)
      return 1;
  }
  if (!data->uncompressible)
    fn202(data, END_OF_FILE_FLAG + (UCHAR_MAX + 1 - MIN_RUN_LENGTH135_IS_3), 0);
  finalise_compresson197(data);
  if (data->uncompressible)
    _231 = 1;
  return _231;
}
