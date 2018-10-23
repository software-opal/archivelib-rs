#include <cstring>

#include "new/expand.hpp"

ALErrors create_expand_data(RExpandData *data, ALStorage &in_storage,
                            ALStorage &out_storage, ssize_t in_length,
                            int compression_level) {
  data->input_store = &in_storage;
  data->output_store = &out_storage;
  data->dat248 = in_length;
  if (compression_level > MAX_COMPRESSION_FACTOR ||
      compression_level < MIN_COMPRESSION_FACTOR) {
    return AL_ILLEGAL_PARAMETER;
  }
  data->dat175 = (int16_t)(1 << compression_level);
  data->dat176 = (int16_t)(data->dat175 - 1);

  data->input_buffer_len = data->dat175 + 2;
  data->input_buffer =
      (uint8_t *)calloc(data->input_buffer_len, sizeof(int8_t));
  data->dat_arr180_len = CONST_N141_IS_511;
  data->dat_arr180 = (uint8_t *)calloc(data->dat_arr180_len, sizeof(uint8_t));
  data->dat_arr181_len = CONST_N152_IS_19;
  data->dat_arr181 = (uint8_t *)calloc(data->dat_arr181_len, sizeof(uint8_t));
  data->dat_arr189_len = 2 * CONST_N141_IS_511 - 1;
  data->dat_arr189 = (uint16_t *)calloc(data->dat_arr189_len, sizeof(uint16_t));
  data->dat_arr190_len = 2 * CONST_N141_IS_511 - 1;
  data->dat_arr190 = (uint16_t *)calloc(data->dat_arr190_len, sizeof(uint16_t));
  data->dat_arr240_len = CONST_N148_IS_4096;
  data->dat_arr240 = (uint16_t *)calloc(data->dat_arr240_len, sizeof(uint16_t));
  data->dat_arr241_len = CONST_N149_IS_256;
  data->dat_arr241 = (uint16_t *)calloc(data->dat_arr241_len, sizeof(uint16_t));
  data->dat_arr242_len = BUFFER_SIZE;
  data->dat_arr242 = (uint8_t *)calloc(data->dat_arr242_len, sizeof(uint8_t));

  if (!data->input_buffer || !data->dat_arr180 || !data->dat_arr181 ||
      !data->dat_arr189 || !data->dat_arr190 || !data->dat_arr240 ||
      !data->dat_arr241 || !data->dat_arr242) {
    return AL_CANT_ALLOCATE_MEMORY;
  }
  return AL_SUCCESS;
}

void free_expand_data(RExpandData *data) {
  if (data->input_buffer)
    delete[] data->input_buffer;
  if (data->dat_arr240)
    delete[] data->dat_arr240;
  if (data->dat_arr241)
    delete[] data->dat_arr241;
  if (data->dat_arr242)
    delete[] data->dat_arr242;
  if (data->dat_arr189)
    delete[] data->dat_arr189;
  if (data->dat_arr190)
    delete[] data->dat_arr190;
  if (data->dat_arr180)
    delete[] data->dat_arr180;
  if (data->dat_arr181)
    delete[] data->dat_arr181;
}
