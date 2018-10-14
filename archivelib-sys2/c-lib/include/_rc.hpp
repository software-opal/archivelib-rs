#ifndef __RC_H
#define __RC_H

#include "_r.h"
#include "stor.h"
#include "status.h"

#if defined(__cplusplus)

class RCompress {
public:
  ALStorage *_161;
  ALStorage *_162;
  int16_t *_163;
  int16_t *_164;
  uint8_t *_165;
  uint8_t *_166;
  uint16_t _167[17];

  int16_t *_177;
  uint8_t *_178;
  uint8_t *_179;
  uint8_t *_180;
  uint8_t *_181;

  uint16_t *_187;
  uint16_t *_188;
  uint16_t *_189;
  uint16_t *_190;
  uint16_t *_191;
  uint16_t *_192;
  uint16_t *_193;
  uint16_t *_194;

  uint64_t _533;
  uint64_t _534;
  int16_t _168;
  int16_t _169;
  int16_t _170;
  int16_t _171;
  int16_t _172;
  int16_t _173;
  int16_t _174;
  int16_t _175;
  int16_t _176;
  uint16_t _182;
  uint16_t _183;
  uint16_t _184;
  uint16_t _185;
  uint16_t _186;
  int32_t _531;

  void fn196();
  void fn197();
  void fn198();
  void fn199(int16_t _200, int16_t _201);
  void fn202(uint16_t _203, uint16_t _204);
  void fn205();
  void fn206();
  void fn207();
  void fn208(int32_t _209, uint16_t _203);
  void fn210();
  int32_t fn211(int32_t _212, uint16_t *_213, uint8_t *_214, uint16_t *_215);
  void fn216(uint16_t *_217);
  void fn218(int16_t _219, int16_t _220, int16_t _221);
  void fn222();
  void fn223(int16_t _203);
  void fn224(uint16_t _204);
  void fn225(int32_t i, uint16_t *_187, int16_t *_177, int16_t _227);
  void fn228(int32_t _229);
  void fn230(int32_t _219, uint8_t *_209, uint16_t *_231);
  void fn232(int32_t i);

public:
  RCompress(ALStorage &_233, ALStorage &_202, int32_t _234, int32_t _235);
  ~RCompress();
  int32_t Compress();
  ALStatus mStatus;

protected:
  RCompress(RCompress &);
  RCompress &operator=(RCompress &);
};

#endif
#endif
