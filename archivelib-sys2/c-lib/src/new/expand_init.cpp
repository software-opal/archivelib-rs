#include <cstring>

#include "new/expand.hpp"

ALErrors create_expand_data(RExpandData *data, ALStorage &in_storage,
                            ALStorage &out_storage, ssize_t in_length,
                            int compression_level) {
  data->dat161 = &in_storage;
  data->dat162 = &out_storage;
  data->dat248 = in_length;
  ;
  if (compression_level > MAX_COMPRESSION_FACTOR ||
      compression_level < MIN_COMPRESSION_FACTOR) {
    return AL_ILLEGAL_PARAMETER;
    data->dat175 = 2;
  } else
    data->dat175 = (int16_t)(1 << compression_level);
  data->dat176 = (int16_t)(data->dat175 - 1);
  data->dat166 = new uint8_t[data->dat175 + 2];
  if (data->dat166)
    memset(data->dat166, 0, (data->dat175 + 2) * sizeof(uint8_t));
  data->dat240 = new uint16_t[CONST_N148_IS_4096];
  if (data->dat240)
    memset(data->dat240, 0, CONST_N148_IS_4096 * sizeof(uint16_t));
  data->dat241 = new uint16_t[CONST_N149_IS_256];
  if (data->dat241)
    memset(data->dat241, 0, CONST_N149_IS_256 * sizeof(uint16_t));
  data->dat242 = new uint8_t[BUFFER_SIZE];
  if (data->dat242)
    memset(data->dat242, 0, BUFFER_SIZE * sizeof(uint8_t));
  data->dat189 = new uint16_t[2 * CONST_N141_IS_511 - 1];
  if (data->dat189)
    memset(data->dat189, 0, (2 * CONST_N141_IS_511 - 1) * sizeof(uint16_t));
  data->dat190 = new uint16_t[2 * CONST_N141_IS_511 - 1];
  if (data->dat190)
    memset(data->dat190, 0, (2 * CONST_N141_IS_511 - 1) * sizeof(uint16_t));
  data->dat180 = new uint8_t[CONST_N141_IS_511];
  data->dat181 = new uint8_t[CONST_N152_IS_19];
  if (!data->dat166 || !data->dat240 || !data->dat241 || !data->dat242 ||
      !data->dat189 || !data->dat190 || !data->dat180 || !data->dat181) {
    return AL_CANT_ALLOCATE_MEMORY;
  }
  return AL_SUCCESS;
}
