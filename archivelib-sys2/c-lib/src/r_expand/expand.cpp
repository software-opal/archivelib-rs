
#include <cassert>

#include "support/expand.hpp"

#include "r_expand.hpp"

int32_t RExpand::Expand() {
  int16_t run_start226;
  int16_t run_length276;
  int16_t byte_or_run_length203;
  int16_t buffer_pos;
  uint8_t *l_uncompressed_buffer278;
  size_t max_size279;
  int16_t size_bitmask280;

  data->error_counter243 = 0;
  data->items_until_next_header = 0;
  data->bits182 = 0;
  data->tmp_bit_buffer245 = 0;
  data->bits_in_buffer172 = 0;
  data->loaded_compressed_data_length246 = 0;

  l_uncompressed_buffer278 = data->uncompressed_buffer;
  max_size279 = data->max_uncompressed_data_size;
  size_bitmask280 = data->max_uncompressed_data_size_bitmask;
  buffer_pos = 0;

  // Seed bits182 with the first 2 bits
  read_bits(2 * CHAR_BIT);

  while (data->error_counter243 < 5) {
    byte_or_run_length203 = get_next_item();
    assert(byte_or_run_length203 <= 0x1FE);
    if (byte_or_run_length203 <= UCHAR_MAX) {
      // byte_or_run_length203 is the decompressed byte
      l_uncompressed_buffer278[buffer_pos] = (uint8_t)byte_or_run_length203;
      if (++buffer_pos >= max_size279) {
        buffer_pos = 0;
        if (data->output_store->WriteBuffer(l_uncompressed_buffer278,
                                            max_size279) != max_size279)
          return false;
      }
    } else {
      // Copy the run of `run_length276` bytes from earlier in the output.
      // byte_or_run_length203 >= 0x100 indicates a flag
      // run_length276 = byte_or_run_length203 - 0x100 + 3; which is the length
      // of the run. Flag value of byte_or_run_length203 ==
      run_length276 =
          (byte_or_run_length203 - (UCHAR_MAX + 1) + MIN_RUN_LENGTH135_IS_3);
      if (run_length276 == END_OF_FILE_FLAG) {
        // byte_or_run_length203 == 0x1FE. End of file.
        break;
      }
      run_start226 =
          (buffer_pos - calculate_run_offset() - 1) & size_bitmask280;
      if (run_start226 < max_size279 - CONST_N140_IS_256 - 1 &&
          buffer_pos < max_size279 - CONST_N140_IS_256 - 1) {
        while (--run_length276 >= 0) {
          l_uncompressed_buffer278[buffer_pos++] =
              l_uncompressed_buffer278[run_start226++];
        }
      } else {
        while (--run_length276 >= 0) {
          l_uncompressed_buffer278[buffer_pos] =
              l_uncompressed_buffer278[run_start226];
          if (++buffer_pos >= max_size279) {
            buffer_pos = 0;
            if (data->output_store->WriteBuffer(l_uncompressed_buffer278,
                                                max_size279) != max_size279)
              return false;
          }
          run_start226 = (int16_t)((run_start226 + 1) & size_bitmask280);
        }
      }
    }
  }
  DE;
  if (buffer_pos != 0)
    data->output_store->WriteBuffer(l_uncompressed_buffer278, buffer_pos);

  return true;
}
