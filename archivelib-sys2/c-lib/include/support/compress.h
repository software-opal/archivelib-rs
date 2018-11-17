#ifndef SUPPORT__COMPRESS_HPP
#define SUPPORT__COMPRESS_HPP

#include "support/debug.h"
#include "new/compress_struct.h"

RCompressData *clone_compress_data(RCompressData *data);
bool diff_compress_data(RCompressData *old_data, RCompressData *new_data);

#ifdef NDEBUG

#define DC                                                                     \
  {}

#else

#define COMPRESS_ABORT(data)                                                   \
  std::cerr << __FILE__ << ":" << __LINE__ << "\n";                            \
  DEBUG_COMPRESS_DATA(std::cerr, data);                                        \
  abort();

#define DEBUG_COMPRESS_DATA(stream, data)                                      \
  stream << "{\"ptr\": " << (intptr_t)(data);                                  \
  WRITE_STORAGE(stream, data, input_store);                                    \
  WRITE_STORAGE(stream, data, output_store);                                   \
                                                                               \
  WRITE_DATA_DEC(stream, data, chars_written);                                 \
  WRITE_DATA_DEC(stream, data, input_length);                                  \
  WRITE_DATA_BOOL(stream, data, uncompressible);                               \
  WRITE_DATA_BOOL(stream, data, fail_uncompressible);                          \
  WRITE_DATA_HEX(stream, data, dat168);                                        \
  WRITE_DATA_HEX(stream, data, dat169);                                        \
  WRITE_DATA_HEX(stream, data, buffer_position);                               \
  WRITE_DATA_HEX(stream, data, bits_buffer_used172);                           \
  WRITE_DATA_HEX(stream, data, dat173);                                        \
  WRITE_DATA_HEX(stream, data, dat174);                                        \
  WRITE_DATA_HEX(stream, data, max_uncompressed_data_size);                    \
  WRITE_DATA_HEX(stream, data, max_uncompressed_data_size_bitmask);            \
  WRITE_DATA_HEX(stream, data, bits_buffer182);                                \
  WRITE_DATA_HEX(stream, data, dat183_IS_CONST_8162);                          \
  WRITE_DATA_HEX(stream, data, array165_counter);                              \
  WRITE_DATA_HEX(stream, data, bitwise_counter185);                            \
  WRITE_DATA_HEX(stream, data, array165_tmp_counter186);                       \
                                                                               \
  WRITE_DATA_ARRAY(stream, data, dat_arr163, bool);                            \
  WRITE_DATA_ARRAY(stream, data, dat_arr164, bool);                            \
  WRITE_DATA_ARRAY(stream, data, dat_arr165, uint8_t);                         \
  WRITE_DATA_ARRAY(stream, data, uncompressed_buffer, uint8_t);                \
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
  WRITE_DATA_ARRAY_PTR(stream, data, dat_arr_cursor178, uint8_t);              \
  WRITE_DATA_ARRAY_PTR(stream, data, dat_arr_cursor187, uint16_t);             \
  WRITE_DATA_ARRAY_PTR(stream, data, dat_arr_cursor188, uint16_t);             \
  stream << "}\n";

#define DC                                                                     \
  {                                                                            \
    /*DEBUG_FILE_HANDLE(fs, data);                                             \
    DEBUG_COMPRESS_DATA(fs, data);*/                                           \
  }

#endif
#endif
