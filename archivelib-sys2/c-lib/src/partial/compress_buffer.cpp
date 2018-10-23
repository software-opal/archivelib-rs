
#include "r_compress.hpp"

void RCompress::write_stored_bits_to_buffer(int16_t arg203) {
  /*
   `arg203` appears to be the bits in the file most of the time
   */
  write_bits_to_buffer(data->dat_arr180[arg203], data->dat_arr192[arg203]);
}

void RCompress::write_bits_to_buffer(int32_t bit_count209, uint16_t bits203) {
  /*

  `bit_count209`: Number of bits to use from `bits203`

  `data->bits_buffer_used172` Number of bits in use in `data->bits_buffer182`.
  */
  // Move the assigned bits into the highest bits of `bits203`
  bits203 = bits203 << (UINT16_BIT - bit_count209);
  // Combine the existing bits with these new bits without overlap
  data->bits_buffer182 |= (uint16_t)(bits203 >> data->bits_buffer_used172);
  data->bits_buffer_used172 += bit_count209;
  if (data->bits_buffer_used172 >= 8) {
    // Highest 8 bits are assigned(at least); save them to the buffer.
    if (data->buffer_position >= BUFFER_SIZE) {
      flush_to_output(data);
    }
    // Take the high bits of bits_buffer182
    data->buffer[data->buffer_position] =
        (uint8_t)(data->bits_buffer182 >> CHAR_BIT);
    data->buffer_position++;
    data->bits_buffer_used172 = data->bits_buffer_used172 - CHAR_BIT;
    if (data->bits_buffer_used172 < CHAR_BIT) {
      // Missing enough bits to do the same thing again.
      // Move the low bits of `data->bits_buffer182` into the high bits.
      data->bits_buffer182 <<= CHAR_BIT;
    } else {
      if (data->buffer_position >= BUFFER_SIZE) {
        flush_to_output(data);
      }
      // Take the low bits of bits_buffer182
      data->buffer[data->buffer_position] = (uint8_t)data->bits_buffer182;
      data->buffer_position++;
      data->bits_buffer_used172 = data->bits_buffer_used172 - CHAR_BIT;
      // Handle any bits that didn't fit the first time we tried.
      data->bits_buffer182 = bits203
                             << (bit_count209 - data->bits_buffer_used172);
    }
  }
}

void RCompress::finalise_compresson197() {
  if (!data->uncompressible)
    fn207();
  finalize_buffer206();
  data->dat183_IS_CONST_8162 = 0;
  data->array165_counter = 0;
}

void RCompress::finalize_buffer206() {
  if (!data->uncompressible) {
    // Write enough bits to clear out any that have been set, without writing a
    // whole new byte if if no bits need clearing
    write_bits_to_buffer(CHAR_BIT - 1, 0);
    if (data->buffer_position) {
      // Flush the data that is waiting
      flush_to_output(data);
    }
  }
  data->buffer_position = 0;
}
