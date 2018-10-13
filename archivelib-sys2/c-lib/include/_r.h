#ifndef _R_H
#define _R_H
#include <limits.h>
#include <stdint.h>
#include "aldefs.h"

typedef uint16_t uint16_t;
typedef uint8_t uint8_t;
typedef uint32_t uint32_t;
#define _132 (CHAR_BIT * sizeof(uint16_t))
#define _133 16
#define _134 '\0'
#define _135 3
#define _136 16384
#define _137 14
#define _138 10
#define _139 8
#define _140 256
#define _141 (UCHAR_MAX + 1 + _140 - _135 + 1 + 1)
#define _142 (_137 + 1)
#define _143 9
#define _144 (_140 + 1)
#define _145 (_133 + 3)
#define _540 5
#define _147 5
#define _148 4096
#define _149 256

#if (1U << _540) <= _142
#error pbit too small
#endif
#if (1U << _147) <= _145
#error tbit too small
#endif

#if _145 > _142
#define _152 _145
#else
#define _152 _142
#endif

#define _153 4096
#define _154 4
#define _155 8192
#define _156 512
#define _157 (-1)
#define _158 128
#define _159 512

#endif
