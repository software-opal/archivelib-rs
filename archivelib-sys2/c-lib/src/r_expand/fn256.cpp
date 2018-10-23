#include <cstdio>
#include <iostream>
#include <cstdlib>

#include "r_expand.hpp"
#include "support/debug.hpp"

void RExpand::fn256(int32_t bits_to_load219) {
  /*
  Reads bits into `data->bits182`.


  */

  printf("\nLoading %i bytes\n", bits_to_load219);
  printf("  bits_to_load219: %i\n", bits_to_load219);
  printf("      data->bits_in_buffer172: %i\n", data->bits_in_buffer172);
  WRITE_BITS(std::cout, "data->bits182", data->bits182);
  printf("\n      data->bits182: %#x\n", data->bits182);
  printf("      data->dat243: %i\n", data->dat243);
  printf("      data->tmp_bit_buffer245: %i\n", data->tmp_bit_buffer245);
  printf("      data->dat246: %i\n", data->dat246);
  printf("      data->dat248: %zi\n", data->dat248);

  while (bits_to_load219 > data->bits_in_buffer172) {
    // This loop loads 1 new byte into `data->tmp_bit_buffer245`(the temporary
    // buffer)
    bits_to_load219 -= data->bits_in_buffer172;
    // Rotate in the remaining bits from the tmp_bit_buffer.
    data->bits182 =
        (data->bits182 << data->bits_in_buffer172) +
        (data->tmp_bit_buffer245 >> (CHAR_BIT - data->bits_in_buffer172));

    WRITE_BITS(std::cout, "data->bits182", data->bits182);
    printf("\n      data->bits182: %#x\n", data->bits182);

    if (data->dat246 <= 0) {
      data->dat_arr_cursor247 = data->compressed_data_buffer242;
      if (data->dat248 >= 0 && data->dat248 < BUFFER_SIZE) {
        data->dat246 = (int16_t)data->input_store->ReadBuffer(data->compressed_data_buffer242,
                                                              data->dat248);
        data->dat248 -= data->dat246;
      } else {
        data->dat246 = (int16_t)data->input_store->ReadBuffer(data->compressed_data_buffer242,
                                                              BUFFER_SIZE);
      }
      if (data->dat246 <= 0)
        data->dat243++;
    }
    data->tmp_bit_buffer245 = *data->dat_arr_cursor247;
    data->dat_arr_cursor247++;
    data->dat246--;
    data->bits_in_buffer172 = CHAR_BIT;
  }

  data->bits_in_buffer172 =
      (int16_t)(data->bits_in_buffer172 - bits_to_load219);
  data->bits182 =
      (uint16_t)((data->bits182 << bits_to_load219) +
                 (data->tmp_bit_buffer245 >> (CHAR_BIT - bits_to_load219)));
  data->tmp_bit_buffer245 <<= bits_to_load219;

  printf("After\n");
  printf("  bits_to_load219: %i\n", bits_to_load219);
  printf("      data->bits_in_buffer172: %i\n", data->bits_in_buffer172);
  WRITE_BITS(std::cout, "data->bits182", data->bits182);
  printf("\n      data->bits182: %#x\n", data->bits182);
  printf("      data->dat243: %i\n", data->dat243);
  printf("      data->tmp_bit_buffer245: %i\n", data->tmp_bit_buffer245);
  printf("      data->dat246: %i\n", data->dat246);
  printf("      data->dat248: %zi\n", data->dat248);
  printf("===========\n");
}
