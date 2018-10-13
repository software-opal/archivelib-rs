#ifndef __RC_H
#define __RC_H

#include "_r.h"

#if defined(__cplusplus)

class RCompress {
private:
  ALStorage *_161;
  ALStorage *_162;
  int16_t *_163;
  int16_t *_164;
  uint8_t *_165;
  uint8_t *_166;
  uint64_t _533;
  uint64_t _534;
  uint16_t _167[17];
  int16_t _168;
  int16_t _169;
  int16_t _170;
  int16_t _171;
  int16_t _172;
  int16_t _173;
  int16_t _174;
  int16_t _175;
  int16_t _176;
  int16_t *_177;
  uint8_t *_178;
  uint8_t *_179;
  uint8_t *_180;
  uint8_t *_181;
  uint16_t _182;
  uint16_t _183;
  uint16_t _184;
  uint16_t _185;
  uint16_t _186;
  uint16_t *_187;
  uint16_t *_188;
  uint16_t *_189;
  uint16_t *_190;
  uint16_t *_191;
  uint16_t *_192;
  uint16_t *_193;
  uint16_t *_194;
  int32_t _531;

public:
  void _196();
  void _197();
  void _198();
  void _199(int16_t _200, int16_t _201);
  void _202(uint16_t _203, uint16_t _204);
  void _205();
  void _206();
  void _207();
  void _208(int32_t _209, uint16_t _203);
  void _210();
  int32_t _211(int32_t _212, uint16_t *_213, uint8_t *_214, uint16_t *_215);
  void _216(uint16_t *_217);
  void _218(int16_t _219, int16_t _220, int16_t _221);
  void _222();
  void _223(int16_t _203);
  void _224(uint16_t _204);
  void _225(int32_t _226, uint16_t *_187, int16_t *_177, int16_t _227);
  void _228(int32_t _229);
  void _230(int32_t _219, uint8_t *_209, uint16_t *_231);
  void _232(int32_t _226);

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
