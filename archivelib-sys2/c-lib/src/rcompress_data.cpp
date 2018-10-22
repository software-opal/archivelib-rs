#include <string>
#include <iostream>
#include <sstream>
#include <string.h>

#include "_rc.hpp"


ALErrors
create_compress_data(RCompressData *data, ALStorage &in_storage,
                     ALStorage &out_storage,
                     ALGreenleafCompressionLevels compression_level_enum,
                     bool fail_uncompressible) {
  int8_t compression_level = ((int8_t)compression_level_enum) + 10;
  data->input_store = &in_storage;
  data->output_store = &out_storage;
  data->fail_uncompressible = fail_uncompressible;
  if (compression_level > MAX_COMPRESSION_FACTOR ||
      compression_level < MIN_COMPRESSION_FACTOR) {
    return AL_ILLEGAL_PARAMETER;
  }
  data->max_input_data_size = (int16_t)(1 << compression_level);
  data->max_input_data_size_minus_one =
      (int16_t)(data->max_input_data_size - 1);
  data->chars_written = 0;
  data->input_length = in_storage.GetSize();

  data->dat_arr163_len = data->max_input_data_size + CONST_N153_IS_4096;
  data->dat_arr163 = (bool *)calloc(data->dat_arr163_len, sizeof(bool));
  data->dat_arr164_len = data->max_input_data_size;
  data->dat_arr164 = (bool *)calloc(data->dat_arr164_len, sizeof(bool));
  data->dat_arr165_len = CONST_N155_IS_8192;
  data->dat_arr165 = (uint8_t *)calloc(data->dat_arr165_len, sizeof(uint8_t));
  data->input_buffer_len = data->max_input_data_size + CONST_N140_IS_256 + 2;
  data->input_buffer =
      (uint8_t *)calloc(data->input_buffer_len, sizeof(uint8_t));
  data->dat_arr167_len = 17;
  data->dat_arr167 = (uint16_t *)calloc(data->dat_arr167_len, sizeof(uint16_t));
  data->dat_arr177_len = CONST_N141_IS_511 + 1;
  data->dat_arr177 = (int16_t *)calloc(data->dat_arr177_len, sizeof(int16_t));
  data->buffer_len = BUFFER_SIZE;
  data->buffer = (uint8_t *)calloc(data->buffer_len, sizeof(uint8_t));
  data->dat_arr180_len = CONST_N141_IS_511;
  data->dat_arr180 = (uint8_t *)calloc(data->dat_arr180_len, sizeof(uint8_t));
  data->dat_arr181_len = CONST_N152_IS_19;
  data->dat_arr181 = (uint8_t *)calloc(data->dat_arr181_len, sizeof(uint8_t));
  data->dat_arr189_len = 2 * CONST_N141_IS_511 - 1;
  data->dat_arr189 = (uint16_t *)calloc(data->dat_arr189_len, sizeof(uint16_t));
  data->dat_arr190_len = 2 * CONST_N141_IS_511 - 1;
  data->dat_arr190 = (uint16_t *)calloc(data->dat_arr190_len, sizeof(uint16_t));
  data->bit_pattern_occurrences191_len = 2 * CONST_N141_IS_511 - 1;
  data->bit_pattern_occurrences191 = (uint16_t *)calloc(
      data->bit_pattern_occurrences191_len, sizeof(uint16_t));
  data->dat_arr192_len = CONST_N141_IS_511;
  data->dat_arr192 = (uint16_t *)calloc(data->dat_arr192_len, sizeof(uint16_t));
  data->dat_arr193_len = 2 * CONST_N142_IS_15 - 1;
  data->dat_arr193 = (uint16_t *)calloc(data->dat_arr193_len, sizeof(uint16_t));
  data->dat_arr194_len = CONST_N152_IS_19;
  data->dat_arr194 = (uint16_t *)calloc(data->dat_arr194_len, sizeof(uint16_t));

  if (!data->dat_arr163 || !data->dat_arr164 || !data->dat_arr165 ||
      !data->input_buffer || !data->dat_arr167 || !data->dat_arr177 ||
      !data->buffer || !data->dat_arr180 || !data->dat_arr181 ||
      !data->dat_arr189 || !data->dat_arr190 ||
      !data->bit_pattern_occurrences191 || !data->dat_arr192 ||
      !data->dat_arr193 || !data->dat_arr194) {
    return AL_CANT_ALLOCATE_MEMORY;
  }
  return AL_SUCCESS;
}

#define DO_CLONE(new_data, old_data, member, type)                             \
  new_data->member = (type *)calloc(new_data->member##_len, sizeof(type));     \
  memcpy(new_data->member, old_data->member,                                   \
         new_data->member##_len * sizeof(type));

RCompressData *clone_compress_data(RCompressData *old_data) {
  RCompressData *new_data = (RCompressData *)malloc(sizeof(RCompressData));
  memcpy(new_data, old_data, sizeof(RCompressData));
  DO_CLONE(new_data, old_data, dat_arr163, bool);
  DO_CLONE(new_data, old_data, dat_arr164, bool);
  DO_CLONE(new_data, old_data, dat_arr165, uint8_t);
  DO_CLONE(new_data, old_data, input_buffer, uint8_t);
  DO_CLONE(new_data, old_data, dat_arr167, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr177, int16_t);
  DO_CLONE(new_data, old_data, buffer, uint8_t);
  DO_CLONE(new_data, old_data, dat_arr180, uint8_t);
  DO_CLONE(new_data, old_data, dat_arr181, uint8_t);
  DO_CLONE(new_data, old_data, dat_arr189, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr190, uint16_t);
  DO_CLONE(new_data, old_data, bit_pattern_occurrences191, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr192, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr193, uint16_t);
  DO_CLONE(new_data, old_data, dat_arr194, uint16_t);

  new_data->dat_arr_cursor178 = NULL;
  new_data->dat_arr_cursor187 = NULL;
  new_data->dat_arr_cursor188 = NULL;

  return new_data;
}

void free_compress_data(RCompressData *data) {
  if (data->dat_arr163) {
    free(data->dat_arr163);
    data->dat_arr163 = NULL;
  }
  if (data->dat_arr164) {
    free(data->dat_arr164);
    data->dat_arr164 = NULL;
  }
  if (data->dat_arr165) {
    free(data->dat_arr165);
    data->dat_arr165 = NULL;
  }
  if (data->input_buffer) {
    free(data->input_buffer);
    data->input_buffer = NULL;
  }
  if (data->dat_arr167) {
    free(data->dat_arr167);
    data->dat_arr167 = NULL;
  }
  if (data->buffer) {
    free(data->buffer);
    data->buffer = NULL;
  }
  if (data->dat_arr177) {
    free(data->dat_arr177);
    data->dat_arr177 = NULL;
  }
  if (data->dat_arr180) {
    free(data->dat_arr180);
    data->dat_arr180 = NULL;
  }
  if (data->dat_arr181) {
    free(data->dat_arr181);
    data->dat_arr181 = NULL;
  }
  if (data->dat_arr189) {
    free(data->dat_arr189);
    data->dat_arr189 = NULL;
  }
  if (data->dat_arr190) {
    free(data->dat_arr190);
    data->dat_arr190 = NULL;
  }
  if (data->bit_pattern_occurrences191) {
    free(data->bit_pattern_occurrences191);
    data->bit_pattern_occurrences191 = NULL;
  }
  if (data->dat_arr192) {
    free(data->dat_arr192);
    data->dat_arr192 = NULL;
  }
  if (data->dat_arr193) {
    free(data->dat_arr193);
    data->dat_arr193 = NULL;
  }
  if (data->dat_arr194) {
    free(data->dat_arr194);
    data->dat_arr194 = NULL;
  }
}

void reset_compress_data(RCompressData *data) {
  ssize_t i;
  data->dat173 = 0;
  data->bits_buffer_used172 = 0;
  data->bits_buffer182 = 0;
  data->buffer_position = 0;
  data->uncompressible = 0;
  data->bitwise_counter185 = 1;
  data->array165_counter = 0;
  data->array165_tmp_counter186 = 0;
  data->dat_arr165[0] = 0;
  data->dat169 = 0;
  data->dat168 = 0;
  data->dat183_IS_CONST_8162 =
      CONST_N155_IS_8192 - (uint16_t)((3 * CHAR_BIT) + 6);

  WRITE_HEX(std::cout, "data->dat183_IS_CONST_8162",
            data->dat183_IS_CONST_8162);

  for (size_t i = 0; i < data->dat_arr192_len; i++) {
    data->dat_arr192[i] = i;
  }
  for (i = 0; i < CONST_N141_IS_511; i++) {
    data->bit_pattern_occurrences191[i] = 0;
  }
  for (i = 0; i < CONST_N142_IS_15; i++) {
    data->dat_arr193[i] = 0;
  }
  for (i = 0; i < CONST_N153_IS_4096; i++) {
    data->dat_arr163[data->max_input_data_size + i] = true;
  }
  for (i = 0; i < data->max_input_data_size; i++) {
    data->dat_arr164[i] = true;
  }
}

void flush_to_output(RCompressData *data) {
  if (data->buffer_position <= 0) {
    return;
  }
  data->chars_written += data->buffer_position;
  if (data->fail_uncompressible && data->chars_written >= data->input_length) {
    data->uncompressible = 1;
  } else {
    data->output_store->WriteBuffer(data->buffer, data->buffer_position);
    memset(data->buffer, 0, data->buffer_position);
  }
  data->buffer_position = 0;
}

void calculate_pointer_depths(uint16_t *left_array_ptr,
                              uint16_t *right_array_ptr,
                              uint16_t *depth_store_ptr, uint16_t depth,
                              int16_t series_start, uint16_t curr_idx) {
  /*
   * Pointer depth calculation?

   * `left_array_ptr` & `right_array_ptr` contain a series(from `series_start`
   to `curr_idx`) of integers that are `< curr_idx`. If they are between
   `series_start` and `curr_idx`, then it's a pointer to another array index.
   Otherwise it's not. This function calculates the number of non-pointer values
   at each depth by following the pointers until a non-pointer, then
   incrementing the count of depth by 1.

   * Note that the pointers will link to the index of both arrays, and need to
   be explored in both arrays. Each value is unique and there are no loops.

   * Does `left_array_ptr` and `right_array_ptr` represent a binary tree?
   */
  if (curr_idx < series_start) {
    depth_store_ptr[MIN(depth, 16)]++;
  } else {
    calculate_pointer_depths(left_array_ptr, right_array_ptr, depth_store_ptr,
                             depth + 1, series_start, left_array_ptr[curr_idx]);
    calculate_pointer_depths(left_array_ptr, right_array_ptr, depth_store_ptr,
                             depth + 1, series_start,
                             right_array_ptr[curr_idx]);
  }
}