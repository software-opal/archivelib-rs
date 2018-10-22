#include <cstring>

#include "new/compress.hpp"

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
