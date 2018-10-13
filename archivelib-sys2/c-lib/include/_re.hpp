#ifndef __RE_H
#define __RE_H

#include "_r.h"
#include "stor.h"
#include "status.h"

#if defined(__cplusplus)

class RExpand {
public:
  ALStorage *_161;
  ALStorage *_162;
  int16_t _175;
  int16_t _176;
  uint8_t *_166;
  uint16_t *_240;
  uint16_t *_241;
  uint8_t *_242;
  uint16_t *_189;
  uint16_t *_190;
  uint8_t *_180;
  uint8_t *_181;
  int16_t _243;
  uint16_t _244;
  uint16_t _182;
  int16_t _172;
  uint8_t _245;
  int16_t _246;
  uint8_t *_247;
  int64_t _248;
  uint16_t _249();
  uint16_t _250();
  void _251();
  uint16_t _252(int32_t _219);
  void _253(int16_t _254, int16_t _220, int16_t _221);
  void _255();
  void _256(int32_t _219);
  void _257();
  void _258(int32_t _259, uint8_t *_260, int32_t _261, uint16_t *_262,
            uint16_t _263);

public:
  RExpand(ALStorage &_233, ALStorage &_202, int64_t _264, int32_t _234);
  ~RExpand();
  int32_t Expand();
  ALStatus mStatus;

protected:
  RExpand(RExpand &);
  RExpand &operator=(RExpand &);
};

#endif
#endif
