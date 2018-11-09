#ifndef SUPPORT__EXPAND_HPP
#define SUPPORT__EXPAND_HPP

#include "support/debug.hpp"
#include "new/expand_struct.hpp"

RExpandData *clone_expand_data(RExpandData *data);
bool diff_expand_data(RExpandData *old_data, RExpandData *new_data);

#define EXPAND_ABORT(data)                                                     \
  std::cerr << __FILE__ << ":" << __LINE__ << "\n";                            \
  DEBUG_COMPRESS_DATA(std::cerr, data);                                        \
  abort();

#define DEBUG_COMPRESS_DATA(stream, data)                                      \
  stream << "{\"ptr\": " << (intptr_t)(data);                                  \
  WRITE_STORAGE(stream, data, input_store);                                    \
  WRITE_STORAGE(stream, data, output_store);                                   \
                                                                               \
  WRITE_DATA_DEC(stream, data, compressed_data_index);                         \
  WRITE_DATA_DEC(stream, data, bits_in_buffer172);                             \
  WRITE_DATA_HEX(stream, data, max_uncompressed_data_size);                    \
  WRITE_DATA_HEX(stream, data, max_uncompressed_data_size_bitmask);            \
  WRITE_DATA_HEX(stream, data, bits182);                                       \
  WRITE_DATA_HEX(stream, data, error_counter243);                              \
  WRITE_DATA_HEX(stream, data, items_until_next_header);                       \
  WRITE_DATA_HEX(stream, data, tmp_bit_buffer245);                             \
  WRITE_DATA_HEX(stream, data, loaded_compressed_data_length246);              \
  WRITE_DATA_HEX(stream, data, compressed_data_length248);                     \
                                                                               \
  WRITE_DATA_ARRAY(stream, data, uncompressed_buffer, uint8_t);                \
  WRITE_DATA_ARRAY(stream, data, dat_arr180, uint8_t);                         \
  WRITE_DATA_ARRAY(stream, data, dat_arr181, uint8_t);                         \
  WRITE_DATA_ARRAY(stream, data, dat_arr189, uint16_t);                        \
  WRITE_DATA_ARRAY(stream, data, dat_arr190, uint16_t);                        \
  WRITE_DATA_ARRAY(stream, data, dat_arr240, uint16_t);                        \
  WRITE_DATA_ARRAY(stream, data, dat_arr241, uint16_t);                        \
  WRITE_DATA_ARRAY(stream, data, compressed_data_buffer242, uint8_t);          \
                                                                               \
  stream << "},\n";

#endif
