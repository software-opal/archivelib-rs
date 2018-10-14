#ifndef _RC_DATA
#define _RC_DATA

#include "_r.h"
#include "stor.h"

typedef struct RCompressData {
  ALStorage *input_store;
  ALStorage *output_store;

  int16_t *dat_arr163;
  size_t dat_arr163_len;
  int16_t *dat_arr164;
  size_t dat_arr164_len;
  uint8_t *dat_arr165;
  size_t dat_arr165_len;
  uint8_t *dat_arr166;
  size_t dat_arr166_len;

  int16_t *dat_arr177;
  size_t dat_arr177_len;
  uint8_t *dat_arr178;
  size_t dat_arr178_len;
  uint8_t *dat_arr179;
  size_t dat_arr179_len;
  uint8_t *dat_arr180;
  size_t dat_arr180_len;
  uint8_t *dat_arr181;
  size_t dat_arr181_len;

  uint16_t *dat_arr187;
  size_t dat_arr187_len;
  uint16_t *dat_arr188;
  size_t dat_arr188_len;
  uint16_t *dat_arr189;
  size_t dat_arr189_len;
  uint16_t *dat_arr190;
  size_t dat_arr190_len;
  uint16_t *dat_arr191;
  size_t dat_arr191_len;
  uint16_t *dat_arr192;
  size_t dat_arr192_len;
  uint16_t *dat_arr193;
  size_t dat_arr193_len;
  uint16_t *dat_arr194;
  size_t dat_arr194_len;

  uint16_t dat_arr167[17];

  size_t chars_written;
  size_t input_length;
  bool uncompressible;
  bool fail_uncompressible;

  int16_t dat168;
  int16_t dat169;
  int16_t dat171;
  int16_t dat172;
  int16_t dat173;
  int16_t dat174;
  int16_t compression_level_bit;
  int16_t compression_level_bit_minus_one;
  uint16_t dat182;
  uint16_t dat183;
  uint16_t dat184;
  uint16_t dat185;
  uint16_t dat186;
} RCompressData;

ALErrors create_compress_data(RCompressData *data, ALStorage &in_storage,
                              ALStorage &out_storage, ALGreenleafCompressionLevels compression_level,
                              bool fail_uncompressible);

void free_compress_data(RCompressData *data);

#endif
