

#include "new/cpp_compat.h"

size_t ALStorage_ReadBuffer(ALStorage *storage, uint8_t *buffer,
                            size_t length) {
  return storage->ReadBuffer(buffer, length);
}
size_t ALStorage_WriteBuffer(ALStorage *storage, uint8_t *buffer,
                             size_t length) {
  return storage->WriteBuffer(buffer, length);
}

int16_t ALStorage_ReadChar(ALStorage *storage) { return storage->ReadChar(); }
int16_t ALStorage_mStatus(ALStorage *storage) { return storage->mStatus; }

int ALStatus_SetError(ALStatus *status, int error, const char *fmt) {
  return status->SetError(error, fmt);
}
