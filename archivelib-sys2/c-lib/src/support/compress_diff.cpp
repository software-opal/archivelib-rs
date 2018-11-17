#ifdef NDEBUG

#include "support/compress.h"

RCompressData *clone_compress_data(RCompressData *data) {
  if (data)
    return NULL;
  else
    return data;
}
bool diff_compress_data(RCompressData *old_data, RCompressData *new_data) {
  return old_data == new_data;
}

#else

#include <string>
#include <iostream>
#include <sstream>
#include <string.h>

#include "support/compress.h"
#include "support/debug.h"
#include "new/compress.h"

#define DO_CLONE(new_data, old_data, member, type)                             \
  new_data->member = (type *)calloc(new_data->member##_len, sizeof(type));     \
  memcpy(new_data->member, old_data->member,                                   \
         new_data->member##_len * sizeof(type));

RCompressData *clone_compress_data(RCompressData *old_data) {
  RCompressData *new_data = (RCompressData *)malloc(sizeof(RCompressData));
  memcpy(new_data, old_data, sizeof(RCompressData));
  DO_CLONE(new_data, old_data, dat_arr163, int16_t);
  DO_CLONE(new_data, old_data, dat_arr164, int16_t);
  DO_CLONE(new_data, old_data, dat_arr165, uint8_t);
  DO_CLONE(new_data, old_data, uncompressed_buffer, uint8_t);
  DO_CLONE(new_data, old_data, dat_arr167, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr177, int16_t);
  DO_CLONE(new_data, old_data, buffer, uint8_t);
  DO_CLONE(new_data, old_data, dat_arr180, uint8_t);
  DO_CLONE(new_data, old_data, dat_arr181, uint8_t);
  DO_CLONE(new_data, old_data, dat_arr189, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr190, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr191, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr192, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr193, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr194, uint16_t);

  new_data->dat_arr_cursor178 = NULL;
  new_data->dat_arr_cursor187 = NULL;
  new_data->dat_arr_cursor188 = NULL;

  return new_data;
}

bool diff_compress_data(RCompressData *old_data, RCompressData *new_data) {
  bool has_changes = false;
  std::stringstream ss;
  char buff[1000];
  sprintf(buff, "    | %40s | %10s | %2s | %10s |\n", "name", "Old", "", "New");
  ss << buff;
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr163);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr164);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr165);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, uncompressed_buffer);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr167);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr177);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, buffer);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr180);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr181);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr189);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr190);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr191);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr192);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr193);
  INLINE_DIFF_ARR(ss, has_changes, old_data, new_data, dat_arr194);

  INLINE_DIFF_VAL(ss, has_changes, "zu", old_data, new_data, chars_written);
  INLINE_DIFF_VAL(ss, has_changes, "zu", old_data, new_data, input_length);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, uncompressible);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data,
                  fail_uncompressible);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, dat168);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, dat169);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, buffer_position);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data,
                  bits_buffer_used172);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, dat173);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, dat174);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, bits_buffer182);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data,
                  dat183_IS_CONST_8162);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, array165_counter);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data, bitwise_counter185);
  INLINE_DIFF_VAL(ss, has_changes, "d", old_data, new_data,
                  array165_tmp_counter186);

  if (!has_changes) {
    ss << "No Changes\n";
  }
  printf("\n%s\n", ss.str().c_str());
  free_compress_data(old_data);
  free(old_data);
  return has_changes;
}

#endif
