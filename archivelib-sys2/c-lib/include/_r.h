#ifndef _R_H
#define _R_H

#include <limits.h>
#include <stdint.h>
#include "aldefs.h"

#define UNUSED_132 (sizeof(uint8_t) * sizeof(uint16_t))
#define CONST__133 16
#define CONST__134 '\0'
#define CONST__135 3
#define CONST__136 16384
#define CONST__137 14
#define CONST__138 10
#define CONST__139 8
#define CONST__140 256
#define CONST__141 (UCHAR_MAX + 1 + CONST__140 - CONST__135 + 1 + 1)
#define CONST__142 (CONST__137 + 1)
#define CONST__143 9
#define CONST__144 (CONST__140 + 1)
#define CONST__145 (CONST__133 + 3)
#define CONST__540 5
#define CONST__147 5
#define CONST__148 4096
#define CONST__149 256
#define CONST__152 CONST__145
#define CONST__153 4096
#define CONST__154 4
#define CONST__155 8192
#define BUFFER_SIZE 512
#define CONST__157 (-1)
#define CONST__158 128
#define CONST__159 512

#define _519                                                                   \
  "Incorrect compression level parameter passed to compressor.  Compression "  \
  "level = %d"
#define _520 "Memory allocation failure in compression startup"
#define _521 "Internal 1 error in Greenleaf Decompression routine"
#define _522 "Internal 2 error in Greenleaf Decompression routine"

#endif
