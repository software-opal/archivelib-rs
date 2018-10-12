#ifndef _SIMPLE_STATUS_H
#define _SIMPLE_STATUS_H

#include <stdlib.h>

typedef struct SimpleStatus {
  int status;
  u_int8_t *data;
  size_t length;
} SimpleStatus;

#ifndef SIMPLE_STATUS_SUCCESS
#define SIMPLE_STATUS_SUCCESS()                                                \
  SimpleStatus { AL_SUCCESS, NULL, 0 }
#endif

#endif
