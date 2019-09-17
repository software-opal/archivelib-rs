
#include "r_expand.hpp"

RExpand::RExpand(ALStorage &in_storage, ALStorage &out_storage,
                 ptrdiff_t in_length, int compression_level) {

  data = (RExpandData *)calloc(1, sizeof(RExpandData));
  ALErrors res = create_expand_data(data, &in_storage, &out_storage, in_length,
                                    compression_level);
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

RExpand::~RExpand() {
  if (data) {
    free_expand_data(data);
    free(data);
    data = NULL;
  }
}
