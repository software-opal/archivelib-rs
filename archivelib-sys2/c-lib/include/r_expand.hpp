#ifndef R_EXPAND_HPP
#define R_EXPAND_HPP

#include "new/expand.hpp"

#ifdef __cplusplus

class RExpand {
public:
  RExpandData *data;

  uint16_t fn249();
  uint16_t fn250();
  void fn251();
  uint16_t fn252(int32_t _219);
  void fn253(int16_t _254, int16_t _220, int16_t _221);
  void fn255();
  void fn256(int32_t _219);
  void fn257();
  void fn258(int32_t _259, uint8_t *_260, int32_t _261, uint16_t *_262,
             uint16_t _263);

  RExpand(ALStorage &_233, ALStorage &_202, size_t _264, int32_t _234);
  ~RExpand();
  int32_t Expand();
  ALStatus mStatus;

protected:
  RExpand(RExpand &);
  RExpand &operator=(RExpand &);
};

#endif
#endif
