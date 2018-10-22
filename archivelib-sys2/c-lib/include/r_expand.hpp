#ifndef R_EXPAND_HPP
#define R_EXPAND_HPP

#include "new/expand.hpp"

#include "aldefs.h"
#include <climits>
#include "stor.h"

#ifdef __cplusplus

class RExpand {
  RExpandData *data;

  // ALStorage *data->dat161;
  // ALStorage *data->dat162;
  // int16_t data->dat175;
  // int16_t data->dat176;
  // uint8_t *data->dat166;
  // uint16_t *data->dat240;
  // uint16_t *data->dat241;
  // uint8_t *data->dat242;
  // uint16_t *data->dat189;
  // uint16_t *data->dat190;
  // uint8_t *data->dat180;
  // uint8_t *data->dat181;
  // int16_t data->dat243;
  // uint16_t data->dat244;
  // uint16_t data->dat182;
  // int16_t data->dat172;
  // uint8_t data->dat245;
  // int16_t data->dat246;
  // uint8_t *data->dat247;
  // ssize_t data->dat248;

  uint16_t fn249();
  uint16_t fn250();
  void fn251();
  uint16_t fn252(int _219);
  void fn253(int16_t _254, int16_t _220, int16_t _221);
  void fn255();
  void fn256(int _219);
  void fn257();
  void fn258(int _259, uint8_t *_260, int _261, uint16_t *_262, uint16_t _263);

public:
  RExpand(ALStorage &_233, ALStorage &_202, ssize_t _264, int _234);
  ~RExpand();
  int Expand();
  ALStatus mStatus;

protected:
  RExpand(RExpand &);
  RExpand &operator=(RExpand &);
};

#endif
#endif
