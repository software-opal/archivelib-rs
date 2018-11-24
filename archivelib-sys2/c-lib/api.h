
#include "include/simple_status.h"
#include <stdlib.h>

#define _CHECK_RETURN_CODE(status, line)                                       \
  {                                                                            \
    int __status_code_##line = (status);                                       \
    if (__status_code_##line != AL_SUCCESS) {                                  \
      return build_error_from_status_code(__status_code_##line);               \
    }                                                                          \
  }
#define CHECK_RETURN_CODE(status) _CHECK_RETURN_CODE(status, __LINE__)

#define _CHECK_AL_STATUS(status, line)                                         \
  {                                                                            \
    ALStatus *__status_##line = &(status);                                     \
    if (__status_##line->GetStatusCode() != AL_SUCCESS) {                      \
      return build_error_from_status_obj(__status_##line);                     \
    }                                                                          \
  }
#define CHECK_AL_STATUS(status) _CHECK_AL_STATUS(status, __LINE__)

typedef SimpleStatus AllocatedMemory;

extern "C" AllocatedMemory compress(u_int8_t *input_buffer, size_t length);
extern "C" AllocatedMemory decompress(u_int8_t *input_buffer, size_t length);

extern "C" void clean(AllocatedMemory *memory);
