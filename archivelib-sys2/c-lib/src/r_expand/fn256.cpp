#include "r_expand.hpp"

void RExpand::fn256(int _219) {
  while (_219 > data->dat172) {
    _219 -= data->dat172;
    data->dat182 = (uint16_t)((data->dat182 << data->dat172) +
                              (data->dat245 >> (CHAR_BIT - data->dat172)));
    if (data->dat246 <= 0) {
      data->dat_arr_cursor247 = data->dat_arr242;
      if (data->dat248 >= 0 && data->dat248 < BUFFER_SIZE) {
        data->dat246 = (int16_t)data->input_store->ReadBuffer(
            data->dat_arr242, (ssize_t)data->dat248);
        data->dat248 -= data->dat246;
      } else
        data->dat246 = (int16_t)data->input_store->ReadBuffer(data->dat_arr242,
                                                              BUFFER_SIZE);
      if (data->dat246 <= 0)
        data->dat243++;
    }
    data->dat245 = *data->dat_arr_cursor247++;
    data->dat246--;
    data->dat172 = CHAR_BIT;
  }
  data->dat172 = (int16_t)(data->dat172 - _219);
  data->dat182 =
      (uint16_t)((data->dat182 << _219) + (data->dat245 >> (CHAR_BIT - _219)));
  data->dat245 <<= _219;
}
