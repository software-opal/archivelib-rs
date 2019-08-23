#include <assert.h>

#include "new/cpp_compat.h"
#include "new/expand.h"
#include "support/debug.h"

// TODO: Make this it's own struct so that nothing else can deal with the
// internals.
void expand_read_bits(RExpandData *data, uint8_t bits_to_load219) {
  /*
  Reads `bits_to_load219` bits into the LSB side of `data->bits182`.
  */
  assert(bits_to_load219 <= 16);

  while (bits_to_load219 > data->bits_in_buffer172) {
    // This loop loads 1 new byte into `data->tmp_bit_buffer245`(the temporary
    // buffer)
    bits_to_load219 -= data->bits_in_buffer172;
    // Rotate in the remaining bits from the tmp_bit_buffer.
    data->bits182 =
        (data->bits182 << data->bits_in_buffer172) +
        (data->tmp_bit_buffer245 >> (CHAR_BIT - data->bits_in_buffer172));

    if (data->loaded_compressed_data_length246 <= 0) {
      data->compressed_data_index = 0;
      if (data->compressed_data_length248 >= 0 &&
          data->compressed_data_length248 < BUFFER_SIZE) {
        data->loaded_compressed_data_length246 = ALStorage_ReadBuffer(
            data->input_store, data->compressed_data_buffer242,
            data->compressed_data_length248);
        data->compressed_data_length248 -=
            data->loaded_compressed_data_length246;
      } else {
        data->loaded_compressed_data_length246 = ALStorage_ReadBuffer(
            data->input_store, data->compressed_data_buffer242, BUFFER_SIZE);
      }
      if (data->loaded_compressed_data_length246 <= 0) {
        data->error_counter243++;
      }
    }
    data->tmp_bit_buffer245 =
        data->compressed_data_buffer242[data->compressed_data_index];
    data->compressed_data_index++;
    data->loaded_compressed_data_length246--;
    data->bits_in_buffer172 = CHAR_BIT;
  }

  data->bits_in_buffer172 =
      (int16_t)(data->bits_in_buffer172 - bits_to_load219);
  data->bits182 =
      (uint16_t)((data->bits182 << bits_to_load219) +
                 (data->tmp_bit_buffer245 >> (CHAR_BIT - bits_to_load219)));
  data->tmp_bit_buffer245 <<= bits_to_load219;
}

uint16_t expand_get_bits(RExpandData *data, uint8_t bits_to_load219) {
  uint16_t bits;
  if (bits_to_load219 == 0) {
    return 0;
  }
  assert(bits_to_load219 <= 16);
  bits = (uint16_t)(data->bits182 >> (2 * CHAR_BIT - bits_to_load219));
  expand_read_bits(data, bits_to_load219);
  return bits;
}
