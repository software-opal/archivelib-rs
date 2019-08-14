#ifndef _SIMPLE_STATUS_H
#define _SIMPLE_STATUS_H

#include <stdlib.h>
#include <stdint.h>

typedef struct SimpleStatus {
  int status;
  uint8_t *data;
  size_t length;
} SimpleStatus;

#define SIMPLE_STATUS_SUCCESS()                                                \
  SimpleStatus { AL_SUCCESS, NULL, 0 }

#endif
