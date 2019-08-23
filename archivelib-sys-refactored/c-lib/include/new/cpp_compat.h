#ifndef _NEW_CPP_COMPAT_H
#define _NEW_CPP_COMPAT_H

#include "stor.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifndef __cplusplus
typedef void ALStorage;
typedef void ALStatus;
typedef enum ALErrors ALErrors;
typedef enum ALGreenleafCompressionLevels ALGreenleafCompressionLevels;
typedef ptrdiff_t ssize_t;

typedef int8_t bool;
#define false 0
#define true !0
#endif

#ifdef __cplusplus
extern "C" {
#endif

size_t ALStorage_ReadBuffer(ALStorage *storage, uint8_t *buffer, size_t length);
size_t ALStorage_WriteBuffer(ALStorage *storage, uint8_t *buffer,
                             size_t length);
int16_t ALStorage_ReadChar(ALStorage *storage);
int16_t ALStorage_mStatus(ALStorage *storage);

int ALStatus_SetError(ALStatus *storage, int error, const char *fmt);

#ifdef __cplusplus
}
#endif

#endif
