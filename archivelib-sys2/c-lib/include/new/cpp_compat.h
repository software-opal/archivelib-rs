#ifndef _NEW_CPP_COMPAT_H
#define _NEW_CPP_COMPAT_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "stor.h"

#ifndef __cplusplus
typedef void ALStorage;
typedef enum ALErrors ALErrors;
typedef enum ALGreenleafCompressionLevels ALGreenleafCompressionLevels;
typedef ptrdiff_t ssize_t;
typedef int8_t bool;
#endif

#ifdef __cplusplus
extern "C" {
#endif

size_t ALStorage_ReadBuffer(ALStorage *storage, uint8_t *buffer, size_t length);
size_t ALStorage_WriteBuffer(ALStorage *storage, uint8_t *buffer,
                             size_t length);
int16_t ALStorage_ReadChar(ALStorage *storage);
int16_t ALStorage_mStatus(ALStorage *storage);

#ifdef __cplusplus
}
#endif

#endif
