#ifndef R_EXPAND_HPP
#define R_EXPAND_HPP

#include "new/expand.h"

#include "aldefs.h"
#include <limits.h>
#include "stor.h"

#ifdef __cplusplus

class RExpand {
  RExpandData *data;

  uint16_t get_next_item();
  uint16_t calculate_run_offset();
  void fn251();
  uint16_t get_bits(uint8_t bits_to_load219);
  void fn253(int16_t _254, int16_t _220, int16_t _221);
  void fn255();
  void read_bits(int32_t bits_to_load219);
  void fn257();
  void fn258(int32_t _259, uint8_t *_260, int32_t _261, uint16_t *_262,
             uint16_t _263);

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
