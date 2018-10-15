
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>

#include "api.h"
#include "memstore.h"
#include "grenengn.h"
#include "enum_rev.hpp"

#ifndef CREATE_BUFFER
#define CREATE_BUFFER(buf_name, input_buffer, length)                          \
  ALMemory *buf_name = new ALMemory(input_buffer, length);             \
  buf_name->Create();                                                          \
  CHECK_AL_STATUS(buf_name->mStatus);
#endif

AllocatedMemory _build_error(int status, std::string data) {
  size_t len = data.length() + 1;
  u_int8_t *raw_data = NULL;
  if (len > 1) {
    raw_data = (u_int8_t *)calloc(len, sizeof(char));
    memcpy(raw_data, data.c_str(), len);
  }
  return AllocatedMemory{
      .status = status,
      .data = raw_data,
      .length = len,
  };
}

AllocatedMemory build_error_from_status_code(int status) {
  std::stringstream stream;
  stream << reverseALErrors(status) << " (" << status << ")";
  return _build_error(status, stream.str());
}

AllocatedMemory build_error_from_status_obj(ALStatus *status) {
  std::stringstream stream;
  stream << reverseALErrors(status->GetStatusCode()) << " (" << status << ") ";
  stream << status->GetStatusDetail();
  return _build_error(status->GetStatusCode(), stream.str());
}

AllocatedMemory build_output(ALStorage *out) {
  out->FlushBuffer();
  CHECK_AL_STATUS(out->mStatus);
  out->Seek(0);
  CHECK_AL_STATUS(out->mStatus);

  size_t data_len = out->GetSize();
  u_int8_t *data = (u_int8_t *)calloc(data_len, sizeof(char));
  size_t actual_len = out->ReadBuffer((unsigned char *)data, data_len);
  out->Close();
  if (out->mStatus.GetStatusCode() != AL_SUCCESS) {
    AllocatedMemory status = build_error_from_status_obj(&out->mStatus);
    delete out;
    free(data);
    return status;
  } else {
    delete out;
  }
  if (data_len != actual_len) {
    uint8_t *tmp = (u_int8_t *)realloc(data, actual_len);
    if (tmp != NULL) {
      data = tmp;
    }
  }

  return AllocatedMemory{
      .status = 0,
      .data = data,
      .length = actual_len,
  };
}

extern "C" AllocatedMemory compress(u_int8_t *input_buffer, size_t length) {
  CREATE_BUFFER(in, input_buffer, length)
  CREATE_BUFFER(out, NULL, 0)

  SimpleStatus status = al_compress(AL_GREENLEAF_LEVEL_4, *in, *out);
  if (status.status != AL_SUCCESS) {
    return status;
  }
  in->Close();
  CHECK_AL_STATUS(in->mStatus);
  delete in;

  return build_output(out);
}
extern "C" AllocatedMemory decompress(u_int8_t *input_buffer, size_t length) {
  CREATE_BUFFER(in, input_buffer, length)
  CREATE_BUFFER(out, NULL, 0)

  SimpleStatus status = al_decompress(AL_GREENLEAF_LEVEL_4, *in, *out);
  if (status.status != AL_SUCCESS) {
    return status;
  }
  in->Close();
  CHECK_AL_STATUS(in->mStatus);
  delete in;

  return build_output(out);
}
extern "C" void clean(AllocatedMemory *memory) {
  if (memory->data) {
    free(memory->data);
    memory->data = NULL;
  }
}
