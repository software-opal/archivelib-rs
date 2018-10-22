#include "r_compress.hpp"

RCompress::RCompress(ALStorage &in_storage, ALStorage &out_storage,
                     ALGreenleafCompressionLevels compression_level,
                     bool fail_uncompressible) {
  data = (RCompressData *)calloc(1, sizeof(RCompressData));
  ALErrors res = create_compress_data(data, in_storage, out_storage,
                                      compression_level, fail_uncompressible);
  switch (res) {
  case AL_SUCCESS:
    break;
  case AL_ILLEGAL_PARAMETER:
    mStatus.SetError(AL_ILLEGAL_PARAMETER, INVALID_COMPRESSION_LEVEL_MSG,
                     compression_level);
    break;
  case AL_CANT_ALLOCATE_MEMORY:
    mStatus.SetError(AL_CANT_ALLOCATE_MEMORY, MEMORY_ALLOCATION_FAILURE_MSG);
    break;
  default:
    mStatus.SetError(res, "Other Error");
  }
}
RCompress::~RCompress() {
  if (data) {
    free_compress_data(data);
    free(data);
    data = NULL;
  }
}
