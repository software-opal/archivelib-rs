#ifndef R_COMPRESS_HPP
#define R_COMPRESS_HPP

#include "new/compress.h"

#ifdef __cplusplus
extern "C" {
#endif

void finalise_compresson197(RCompressData *data);
void fn198(RCompressData *data);
void fn199(RCompressData *data, int16_t arg200, int16_t arg201);
void fn202(RCompressData *data, uint16_t bits203, uint16_t arg204);
void finalize_buffer206(RCompressData *data);
void fn207(RCompressData *data);
void write_bits_to_buffer(RCompressData *data, int32_t arg209,
                          uint16_t bits203);
int32_t fn211(RCompressData *data, int32_t arg212, uint16_t *arg213,
              uint8_t *arg214, uint16_t *arg215);
void fn216(RCompressData *data, uint16_t *arg217);
void fn218(RCompressData *data, int16_t length219, int16_t arg220,
           int16_t arg221);
void fn222(RCompressData *data);
void write_stored_bits_to_buffer(RCompressData *data, int16_t bits203);
void fn224(RCompressData *data, uint16_t arg204);
void fn225(RCompressData *data, int32_t i, uint16_t *arg187, int16_t *arg177,
           int16_t arg227);
void fn228(RCompressData *data, int32_t arg229);
void fn230(RCompressData *data, int32_t length219, uint8_t *arg209,
           uint16_t *arg231);

bool Compress(RCompressData *data);

#ifdef __cplusplus
}
#endif

#if defined(__cplusplus)

class RCompress {
public:
  RCompressData *data;

  RCompress(ALStorage &arg233, ALStorage &arg202,
            ALGreenleafCompressionLevels arg234, bool arg235);
  ~RCompress();
  ALStatus mStatus;

protected:
  RCompress(RCompress &);
  RCompress &operator=(RCompress &);
};

#endif
#endif
