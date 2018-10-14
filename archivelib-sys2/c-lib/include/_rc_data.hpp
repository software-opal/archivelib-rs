#ifndef _RC_DATA
#define _RC_DATA

#include "_r.h"
#include "stor.h"

typedef struct RCompressData {
  ALStorage *input_store;
  ALStorage *output_store;

  bool *dat_arr163;
  bool *dat_arr164;
  uint8_t *dat_arr165;
  uint8_t *dat_arr166;
  uint16_t *dat_arr167;
  int16_t *dat_arr177;
  uint8_t *buffer;
  uint8_t *dat_arr180;
  uint8_t *dat_arr181;
  uint16_t *dat_arr189;
  uint16_t *dat_arr190;
  uint16_t *dat_arr191;
  uint16_t *dat_arr192;
  uint16_t *dat_arr193;
  uint16_t *dat_arr194;

  size_t dat_arr163_len;
  size_t dat_arr164_len;
  size_t dat_arr165_len;
  size_t dat_arr166_len;
  size_t dat_arr167_len;
  size_t dat_arr177_len;
  size_t buffer_len;
  size_t dat_arr180_len;
  size_t dat_arr181_len;
  size_t dat_arr189_len;
  size_t dat_arr190_len;
  size_t dat_arr191_len;
  size_t dat_arr192_len;
  size_t dat_arr193_len;
  size_t dat_arr194_len;

  uint8_t *dat_arr_cursor178;
  uint16_t *dat_arr_cursor187;
  uint16_t *dat_arr_cursor188;

  size_t chars_written;
  size_t input_length;
  bool uncompressible;
  bool fail_uncompressible;

  int16_t dat168;
  int16_t dat169;
  int16_t buffer_position;
  int16_t dat172;
  int16_t dat173;
  int16_t dat174;
  int16_t max_input_data_size;
  int16_t max_input_data_size_minus_one;
  uint16_t dat182;
  uint16_t dat183;
  uint16_t dat184;
  uint16_t dat185;
  uint16_t dat186;
} RCompressData;

ALErrors create_compress_data(RCompressData *data, ALStorage &in_storage,
                              ALStorage &out_storage,
                              ALGreenleafCompressionLevels compression_level,
                              bool fail_uncompressible);

void free_compress_data(RCompressData *data);
void reset_compress_data(RCompressData *data);

#ifdef NDEBUG
#define DEBUG_COMPRESS_DATA(stream, data) ((void *)0);
#else
#include "_r_debug.hpp"

#define DEBUG_COMPRESS_DATA(stream, data)                                      \
  stream << "{\"ptr\": " << (intptr_t)(data);                                  \
  WRITE_STORAGE(stream, data, input_store);                                    \
  WRITE_STORAGE(stream, data, output_store);                                   \
                                                                               \
  WRITE_DEC(stream, data, chars_written);                                      \
  WRITE_DEC(stream, data, input_length);                                       \
  WRITE_BOOL(stream, data, uncompressible);                                    \
  WRITE_BOOL(stream, data, fail_uncompressible);                               \
  WRITE_HEX(stream, data, dat168);                                             \
  WRITE_HEX(stream, data, dat169);                                             \
  WRITE_HEX(stream, data, buffer_position);                                    \
  WRITE_HEX(stream, data, dat172);                                             \
  WRITE_HEX(stream, data, dat173);                                             \
  WRITE_HEX(stream, data, dat174);                                             \
  WRITE_HEX(stream, data, max_input_data_size);                                \
  WRITE_HEX(stream, data, max_input_data_size_minus_one);                      \
  WRITE_HEX(stream, data, dat182);                                             \
  WRITE_HEX(stream, data, dat183);                                             \
  WRITE_HEX(stream, data, dat184);                                             \
  WRITE_HEX(stream, data, dat185);                                             \
  WRITE_HEX(stream, data, dat186);                                             \
                                                                               \
  WRITE_DATA_ARRAY(stream, data, dat_arr163, int16_t);                         \
  WRITE_DATA_ARRAY(stream, data, dat_arr164, int16_t);                         \
  WRITE_DATA_ARRAY(stream, data, dat_arr165, uint8_t);                         \
  WRITE_DATA_ARRAY(stream, data, dat_arr166, uint8_t);                         \
  WRITE_DATA_ARRAY(stream, data, dat_arr167, uint16_t);                        \
  WRITE_DATA_ARRAY(stream, data, dat_arr177, int16_t);                         \
  WRITE_DATA_ARRAY(stream, data, buffer, uint8_t);                             \
  WRITE_DATA_ARRAY(stream, data, dat_arr180, uint8_t);                         \
  WRITE_DATA_ARRAY(stream, data, dat_arr181, uint8_t);                         \
  WRITE_DATA_ARRAY(stream, data, dat_arr189, uint16_t);                        \
  WRITE_DATA_ARRAY(stream, data, dat_arr190, uint16_t);                        \
  WRITE_DATA_ARRAY(stream, data, dat_arr191, uint16_t);                        \
  WRITE_DATA_ARRAY(stream, data, dat_arr192, uint16_t);                        \
  WRITE_DATA_ARRAY(stream, data, dat_arr193, uint16_t);                        \
  WRITE_DATA_ARRAY(stream, data, dat_arr194, uint16_t);                        \
                                                                               \
  WRITE_DATA_ARRAY_PTR(stream, data, dat_arr_cursor178, uint8_t);                     \
  WRITE_DATA_ARRAY_PTR(stream, data, dat_arr_cursor187, uint16_t);                    \
  WRITE_DATA_ARRAY_PTR(stream, data, dat_arr_cursor188, uint16_t);                    \
  stream << "},\n";
#endif

#endif
