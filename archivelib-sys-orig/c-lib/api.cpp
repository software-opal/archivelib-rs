
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>

#include "api.h"
#include "grenengn.h"
#include "memstore.h"
#include "enum_rev.hpp"

#ifndef CREATE_BUFFER
#define CREATE_BUFFER(buf_name, name, input_buffer, length)                    \
  ALMemory *buf_name =                                                         \
      new ALMemory(name.c_str(), (char *)input_buffer, length, AL_MIXED);      \
  buf_name->Create();                                                          \
  CHECK_AL_STATUS(buf_name->mStatus);
#endif

AllocatedMemory _build_error(int status, std::string data) {
  size_t len = data.length() + 1;
  uint8_t *raw_data = NULL;
  if (len > 1) {
    raw_data = (uint8_t *)calloc(len, sizeof(char));
    memcpy(raw_data, data.c_str(), len);
  }
  AllocatedMemory m = {};
  m.status = status;
  m.data = raw_data;
  m.length = len;
  return m;
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
  uint8_t *data = (uint8_t *)calloc(data_len, sizeof(char));
  size_t actual_len = out->ReadBuffer((unsigned char *)data, data_len);
  out->Close();
  if (out->mStatus.GetStatusCode() != AL_SUCCESS) {
    delete out;
    free(data);
    return build_error_from_status_obj(&out->mStatus);
  }
  delete out;
  if (data_len != actual_len) {
    data = (uint8_t *)realloc(data, actual_len);
  }

  AllocatedMemory m = {};
  m.status = 0;
  m.data = data;
  m.length = actual_len;
  return m;
}

extern "C" AllocatedMemory compress(uint8_t *input_buffer, size_t length, uint8_t compression_level) {
  std::string name = "compress";
  CREATE_BUFFER(in, name, input_buffer, length)
  CREATE_BUFFER(out, name, NULL, 0)

  // Default is AL_GREENLEAF_LEVEL_0
  ALGreenleafEngine *engn = new ALGreenleafEngine(compression_level , false);
  engn->Compress(*in, *out);
  CHECK_AL_STATUS(engn->mStatus);
  delete engn;
  in->Close();
  CHECK_AL_STATUS(in->mStatus);
  delete in;

  return build_output(out);
}
extern "C" AllocatedMemory decompress(uint8_t *input_buffer, size_t length, uint8_t compression_level) {
  std::string name = "decompress";
  CREATE_BUFFER(in, name, input_buffer, length)
  CREATE_BUFFER(out, name, NULL, 0)

    // Default is AL_GREENLEAF_LEVEL_0
  ALGreenleafEngine *engn = new ALGreenleafEngine(compression_level, false);
  engn->Decompress(*in, *out);
  CHECK_AL_STATUS(engn->mStatus);
  delete engn;
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
