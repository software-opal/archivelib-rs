#ifdef NDEBUG

#include "support/expand.h"

RExpandData *clone_expand_data(RExpandData *data) {
  if (data)
    return NULL;
  else
    return data;
}
bool diff_expand_data(RExpandData *old_data, RExpandData *new_data) {
  return old_data == new_data;
}

#else

#include <iostream>
#include <sstream>
#include <string.h>
#include <string>

#include "new/expand.h"
#include "support/debug.h"
#include "support/expand.h"

#define DO_CLONE(new_data, old_data, member, type)                             \
  new_data->member = (type *)calloc(new_data->member##_len, sizeof(type));     \
  memcpy(new_data->member, old_data->member,                                   \
         new_data->member##_len * sizeof(type));

RExpandData *clone_expand_data(RExpandData *old_data) {
  RExpandData *new_data = (RExpandData *)malloc(sizeof(RExpandData));
  memcpy(new_data, old_data, sizeof(RExpandData));
  DO_CLONE(new_data, old_data, uncompressed_buffer, uint8_t);
  DO_CLONE(new_data, old_data, dat_arr180, uint8_t);
  DO_CLONE(new_data, old_data, dat_arr181, uint8_t);
  DO_CLONE(new_data, old_data, dat_arr189, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr190, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr240, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr241, uint16_t);

  new_data->compressed_data_buffer242 = NULL;

  return new_data;
}

bool diff_expand_data(RExpandData *old_data, RExpandData *new_data) {
  bool has_changes = false;
  std::stringstream ss;
  char buff[1000];
  sprintf(buff, "    | %40s | %10s | %2s | %10s |\n", "name", "Old", "", "New");
  ss << buff;
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, uncompressed_buffer);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr180);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr181);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr189);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr190);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr240);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr241);

  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, bits_in_buffer172);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, bits182);
  INLINE_DIFF_VAL(ss, has_changes, "zu", old_data, new_data,
                  compressed_data_index);
  INLINE_DIFF_VAL(ss, has_changes, "zu", old_data, new_data,
                  compressed_data_length248);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, error_counter243);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data,
                  items_until_next_header);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data,
                  loaded_compressed_data_length246);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data,
                  max_uncompressed_data_size_bitmask);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data,
                  max_uncompressed_data_size);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, tmp_bit_buffer245);

  if (!has_changes) {
    ss << "No Changes\n";
  }
  printf("\n%s\n", ss.str().c_str());
  free_expand_data(old_data);
  free(old_data);
  return has_changes;
}

#endif
