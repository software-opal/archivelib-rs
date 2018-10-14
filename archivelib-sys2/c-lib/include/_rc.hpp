#ifndef __RC_H
#define __RC_H

#include "_r.h"
#include "_rc_data.hpp"
#include "stor.h"
#include "status.h"

#if defined(__cplusplus)

class RCompress {
public:
  RCompressData *data;

  void fn196();
  void fn197();
  void fn198();
  void fn199(int16_t _200, int16_t _201);
  void fn202(uint16_t _203, uint16_t _204);
  void fn205();
  void fn206();
  void fn207();
  void fn208(int32_t _209, uint16_t _203);
  void flush_to_output();
  int32_t fn211(int32_t _212, uint16_t *_213, uint8_t *_214, uint16_t *_215);
  void fn216(uint16_t *_217);
  void fn218(int16_t _219, int16_t _220, int16_t _221);
  void fn222();
  void fn223(int16_t _203);
  void fn224(uint16_t _204);
  void fn225(int32_t i, uint16_t *arg187, int16_t *arg177, int16_t _227);
  void fn228(int32_t _229);
  void fn230(int32_t _219, uint8_t *_209, uint16_t *_231);
  void fn232(int32_t i);

public:
  RCompress(ALStorage &_233, ALStorage &_202, ALGreenleafCompressionLevels _234,
            bool _235);
  ~RCompress();
  int32_t Compress();
  ALStatus mStatus;

protected:
  RCompress(RCompress &);
  RCompress &operator=(RCompress &);
};

#endif
#endif
