
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <mutex>

#include "emb-compress-ext.h"
#include "api.h"

std::mutex mtx;

AllocatedMemory build_output(int status, u_int8_t *data, size_t length) {
  if (status < 0) {
    length = 0;
  }
  if (length == 0) {
    free(data);
    return AllocatedMemory{.status = status, .data = NULL, .length = length};
  } else {
    data = (u_int8_t *)realloc(data, length);
    return AllocatedMemory{.status = status, .data = data, .length = length};
  }
}

extern "C" AllocatedMemory compress(u_int8_t *input_buffer, size_t length) {
  mtx.lock();

  u_int8_t *out = (u_int8_t *)calloc(1 << 16, sizeof(u_int8_t));
  int real_length = husCompress(input_buffer, length, out, 10, 0);

  mtx.unlock();
  return build_output(0, out, real_length);
}

extern "C" AllocatedMemory decompress(u_int8_t *input_buffer, size_t length) {
  mtx.lock();

  u_int8_t *out = (u_int8_t *)calloc(1 << 16, sizeof(u_int8_t));
  int real_length = husExpand(input_buffer, out, length, 10);

  mtx.unlock();
  return build_output(0, out, real_length);
}

extern "C" void clean(AllocatedMemory *memory) {
  if (memory->data) {
    free(memory->data);
    memory->data = NULL;
  }
}
