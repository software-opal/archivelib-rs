#include <string.h>
#include <stdlib.h>

#include "new/expand.h"

ALErrors create_expand_data(RExpandData *data, ALStorage *in_storage,
                            ALStorage *out_storage, ssize_t in_length,
                            int compression_level) {
  data->input_store = in_storage;
  data->output_store = out_storage;
  data->compressed_data_length248 = in_length;
  if (compression_level > MAX_COMPRESSION_FACTOR ||
      compression_level < MIN_COMPRESSION_FACTOR) {
    return AL_ILLEGAL_PARAMETER;
  }
  data->max_uncompressed_data_size = 1 << compression_level;
  data->max_uncompressed_data_size_bitmask =
      data->max_uncompressed_data_size - 1;

  data->uncompressed_buffer_len = data->max_uncompressed_data_size + 2;
  data->uncompressed_buffer =
      (uint8_t *)calloc(data->uncompressed_buffer_len, sizeof(int8_t));
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
  data->compressed_data_buffer242_len = BUFFER_SIZE;
  data->compressed_data_buffer242 =
      (uint8_t *)calloc(data->compressed_data_buffer242_len, sizeof(uint8_t));

  if (!data->uncompressed_buffer || !data->dat_arr180 || !data->dat_arr181 ||
      !data->dat_arr189 || !data->dat_arr190 || !data->dat_arr240 ||
      !data->dat_arr241 || !data->compressed_data_buffer242) {
    return AL_CANT_ALLOCATE_MEMORY;
  }
  return AL_SUCCESS;
}

void free_expand_data(RExpandData *data) {
  if (data->uncompressed_buffer) {
    free(data->uncompressed_buffer);
    data->uncompressed_buffer = NULL;
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
  if (data->dat_arr240) {
    free(data->dat_arr240);
    data->dat_arr240 = NULL;
  }
  if (data->dat_arr241) {
    free(data->dat_arr241);
    data->dat_arr241 = NULL;
  }
  if (data->compressed_data_buffer242) {
    free(data->compressed_data_buffer242);
    data->compressed_data_buffer242 = NULL;
  }
}
