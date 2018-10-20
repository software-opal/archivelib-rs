#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#include "_r_debug.hpp"
#include <string>
#include <iostream>
#include <sstream>

RCompress::RCompress(ALStorage &in_storage, ALStorage &out_storage,
                     ALGreenleafCompressionLevels compression_level,
                     bool fail_uncompressible) {
  data = (RCompressData *)calloc(1, sizeof(RCompressData));
  ALErrors res = create_compress_data(data, in_storage, out_storage,
                                      compression_level, fail_uncompressible);
  switch (res) {
  case AL_SUCCESS:
    break;
  case AL_ILLEGAL_PARAMETER:
    mStatus.SetError(AL_ILLEGAL_PARAMETER, ERROR_MESSAGE_N519,
                     compression_level);
    break;
  case AL_CANT_ALLOCATE_MEMORY:
    mStatus.SetError(AL_CANT_ALLOCATE_MEMORY, ERROR_MESSAGE_N520);
    break;
  default:
    mStatus.SetError(res, "Other Error");
  }
}
RCompress::~RCompress() {
  if (data) {
    free_compress_data(data);
    free(data);
    data = NULL;
  }
}

void RCompress::fn225(int32_t i, uint16_t *arg187, int16_t *arg177,
                      int16_t arg227) {
  /*
    arg187 == data->dat_arr_cursor187, arg177 == data->dat_arr177

    arg187 can be `dat_arr191` or a local variable
    arg177 is a array of index pointers to arg187.

    This is some sort of rotation function in arg177.
  */
  // std::cout << "\n";
  // std::cout << "fn225: i=" << i << "; arg227=" << arg227 << "\n";
  // WRITE_ARRAY_PTR(std::cout, data, "arg177", arg177, int16_t);
  // std::cout << "\n";

  int32_t local276, local289;
  local289 = arg177[i];
  while ((local276 = 2 * i) <= arg227) {
    // std::cout << "  Considering " << local276 << "\n";
    if (local276 < arg227) {
      // std::cout << "   Inside bounds of " << arg227 << "\n";
      // std::cout << "   Lookup arg187[arg177[local276]] = arg187[" <<
      // arg177[local276] << "] = " << arg187[arg177[local276]] << "\n";
      // std::cout << "   Lookup arg187[arg177[local276+1]] = arg187[" <<
      // arg177[local276+1] << "] = " << arg187[arg177[local276+1]] << "\n";
      if (arg187[arg177[local276]] > arg187[arg177[local276 + 1]]) {
        // std::cout << "    Left greater than right; increment local276" <<
        // "\n";
        local276++;
      }
    }

    // std::cout << "  Lookup arg187[local289] = arg187[" << local289 << "] = "
    // << arg187[local289] << "\n"; std::cout << "  Lookup
    // arg187[arg177[local276]] = arg187[" << arg177[local276] << "] = " <<
    // arg187[arg177[local276]] << "\n";

    if (arg187[local289] <= arg187[arg177[local276]]) {
      // std::cout << "  Fin\n";
      break;
    }
    // std::cout << "  Moved " << local276 << " to " << i << "\n";
    arg177[i] = arg177[local276];
    i = local276;
  }
  arg177[i] = (uint16_t)local289;
  // WRITE_ARRAY_PTR(std::cout, data, "arg177", arg177, int16_t);
  // std::cout << "\n";
}

void RCompress::fn224(uint16_t arg204) {
  uint16_t local203, local457;
  local203 = 0;
  local457 = arg204;
  while (local457 != 0) {
    local203++;
    local457 = local457 >> 1;
    // Who knows what goes through here.
    ABORT(data);
  }
  write_bits_to_buffer(data->dat_arr181[local203], data->dat_arr194[local203]);
  if (local203 > 1)
    write_bits_to_buffer(local203 - 1, arg204);
}

void RCompress::fn228(int32_t arg229) {
  size_t i, j, cursor_idx = 0;
  uint32_t local458;
  memset(data->dat_arr167, 0, 17 * sizeof(uint16_t));

  calculate_pointer_depths(data->dat_arr189, data->dat_arr190, data->dat_arr167,
                           0, data->dat174, arg229);

  local458 = 0;
  for (i = 1; i < 17; i++) {
    local458 += data->dat_arr167[i] << (16 - i);
  }
  if (local458 != 0x10000) {
    // This appears to be an incredibly rare event.
    std::cout << "IS THIS A CASE?";
    WRITE_DATA_ARRAY(std::cout, data, dat_arr167, uint16_t);
    std::cout << "\n";
    ABORT(data);
    while (local458 != (1U << 16)) {
      data->dat_arr167[16]--;
      for (i = 15; i > 0; i--) {
        if (data->dat_arr167[i] != 0) {
          data->dat_arr167[i]--;
          data->dat_arr167[i + 1] = (uint16_t)(data->dat_arr167[i + 1] + 2);
          break;
        }
      }
      local458--;
    }
  }

  /*
    Called twice per compression.
    1) dat_arr_cursor178 == dat_arr180 && dat_arr_cursor188 == dat_arr192
    2) dat_arr_cursor178 == dat_arr181 && dat_arr_cursor188 == dat_arr194

    The cursor appears to be unused beyond this method so refactored to use
    array indexing instead of pointer math.

    This outer loop needs to go backwards so elements of 188 are accessed in the
    correct order.
  */
  for (i = 16; i > 0; i--) {
    for (j = 0; j < data->dat_arr167[i]; j++, cursor_idx++) {
      data->dat_arr_cursor178[data->dat_arr_cursor188[cursor_idx]] = (uint8_t)i;
    }
  }
}

void RCompress::fn230(int32_t length219, uint8_t *arg209, uint16_t *arg231) {
  /*
  Called twice:
  1) arg209 == data->dat_arr180 && arg231 == data->dat_arr192
  2) arg209 == data->dat_arr181 && arg231 == data->dat_arr194

  converts the depth counts stored in `dat_arr167` into start values.
  then goes loops over arg231 to store the start value.
  */
  int32_t i;
  uint16_t local288[18];
  memset(local288, 0, 18 * sizeof(uint16_t));
  memset(arg231, 0, length219 * sizeof(uint16_t));

  for (i = 1; (i + 1) < 18; i++)
    local288[i + 1] = (uint16_t)((local288[i] + data->dat_arr167[i]) << 1);

  for (i = 0; i < length219; i++) {
    uint16_t value = local288[arg209[i]];
    arg231[i] = value;
    local288[arg209[i]]++;
  }
}

void RCompress::finalise_compresson197() {
  if (!data->uncompressible)
    fn207();
  finalize_buffer206();
  data->dat183 = 0;
  data->dat184 = 0;
}
void RCompress::finalize_buffer206() {
  if (!data->uncompressible) {
    write_bits_to_buffer(CHAR_BIT - 1, 0);
    if (data->buffer_position)
      flush_to_output(data);
  }
  data->buffer_position = 0;
}
void RCompress::fn223(int16_t arg203) {
  /*
   `arg203` appears to be the bits in the file most of the time
   */
  // printf("fn223: arg203=%#x; data->dat_arr180[arg203]=%#04x, "
  //        "data->dat_arr192[arg203]=%#06x\n",
  //        arg203, data->dat_arr180[arg203], data->dat_arr192[arg203]);
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
