
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "api.h"
#include "enum_rev.hpp"
#include "grenengn.h"
#include "memstore.h"

#ifndef CREATE_BUFFER
#define CREATE_BUFFER(buf_name, input_buffer, length)                          \
  ALMemory *buf_name = new ALMemory(input_buffer, length);                     \
  buf_name->Create();                                                          \
  CHECK_AL_STATUS(buf_name->mStatus);
#endif

AllocatedMemory2 _build_error(int status, std::string data) {
  size_t len = data.length() + 1;
  uint8_t *raw_data = NULL;
  if (len > 1) {
    raw_data = (uint8_t *)calloc(len, sizeof(char));
    memcpy(raw_data, data.c_str(), len);
  }
  return AllocatedMemory2{
      .status = status,
      .data = raw_data,
      .length = len,
  };
}

AllocatedMemory2 build_error_from_status_code(int status) {
  std::stringstream stream;
  stream << reverseALErrors(status) << " (" << status << ")";
  return _build_error(status, stream.str());
}

AllocatedMemory2 build_error_from_status_obj(ALStatus *status) {
  std::stringstream stream;
  stream << reverseALErrors(status->GetStatusCode()) << " (" << status << ") ";
  stream << status->GetStatusDetail();
  return _build_error(status->GetStatusCode(), stream.str());
}

AllocatedMemory2 build_output(ALStorage *out) {
  out->FlushBuffer();
  CHECK_AL_STATUS(out->mStatus);
  out->Seek(0);
  CHECK_AL_STATUS(out->mStatus);

  size_t data_len = out->GetSize();
  uint8_t *data = (uint8_t *)calloc(data_len, sizeof(char));
  size_t actual_len = out->ReadBuffer((uint8_t *)data, data_len);
  out->Close();
  if (out->mStatus.GetStatusCode() != AL_SUCCESS) {
    AllocatedMemory2 status = build_error_from_status_obj(&out->mStatus);
    delete out;
    free(data);
    return status;
  } else {
    delete out;
  }
  if (data_len != actual_len) {
    uint8_t *tmp = (uint8_t *)realloc(data, actual_len);
    if (tmp != NULL) {
      data = tmp;
    }
  }

  return AllocatedMemory2{
      .status = 0,
      .data = data,
      .length = actual_len,
  };
}

extern "C" AllocatedMemory2 compress2(uint8_t *input_buffer, size_t length, uint8_t compression_level) {
  CREATE_BUFFER(in, input_buffer, length)
  CREATE_BUFFER(out, NULL, 0)

  SimpleStatus status = al_compress((ALGreenleafCompressionLevels) compression_level, *in, *out);
  if (status.status != AL_SUCCESS) {
    return status;
  }
  in->Close();
  CHECK_AL_STATUS(in->mStatus);
  delete in;

  return build_output(out);
}
extern "C" AllocatedMemory2 decompress2(uint8_t *input_buffer, size_t length, uint8_t compression_level) {
  CREATE_BUFFER(in, input_buffer, length)
  CREATE_BUFFER(out, NULL, 0)

  SimpleStatus status = al_decompress((ALGreenleafCompressionLevels) compression_level, *in, *out);
  if (status.status != AL_SUCCESS) {
    return status;
  }
  in->Close();
  CHECK_AL_STATUS(in->mStatus);
  delete in;

  return build_output(out);
}
extern "C" void clean2(AllocatedMemory2 *memory) {
  if (memory->data) {
    free(memory->data);
    memory->data = NULL;
  }
}
