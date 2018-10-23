#ifndef R_EXPAND_HPP
#define R_EXPAND_HPP

#include "new/expand.hpp"

#include "aldefs.h"
#include <climits>
#include "stor.h"

#ifdef __cplusplus

class RExpand {
  RExpandData *data;

  // ALStorage *data->input_store;
  // ALStorage *data->output_store;
  // int16_t data->dat175;
  // int16_t data->dat176;
  // uint8_t *data->uncompressed_buffer;
  // uint16_t *data->dat_arr240;
  // uint16_t *data->dat_arr241;
  // uint8_t *data->compressed_data_buffer242;
  // uint16_t *data->dat_arr189;
  // uint16_t *data->dat_arr190;
  // uint8_t *data->dat_arr180;
  // uint8_t *data->dat_arr181;
  // int16_t data->dat243;
  // uint16_t data->dat244;
  // uint16_t data->bits182;
  // int16_t data->bits_in_buffer172;
  // uint8_t data->tmp_bit_buffer245;
  // int16_t data->dat246;
  // uint8_t *data->dat_arr_cursor247;
  // ssize_t data->dat248;

  uint16_t fn249();
  uint16_t fn250();
  void fn251();
  uint16_t fn252(int32_t bits_to_load219);
  void fn253(int16_t _254, int16_t _220, int16_t _221);
  void fn255();
  void fn256(int32_t bits_to_load219);
  void fn257();
  void fn258(int32_t _259, uint8_t *_260, int32_t _261, uint16_t *_262, uint16_t _263);

public:
  RExpand(ALStorage &_233, ALStorage &_202, ssize_t _264, int32_t _234);
  ~RExpand();
  int32_t Expand();
  ALStatus mStatus;

protected:
  RExpand(RExpand &);
  RExpand &operator=(RExpand &);
};

#endif
#endif
