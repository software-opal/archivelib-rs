#ifndef _GRENENGN_H
#define _GRENENGN_H

#include "aldefs.h"
#include "simple_status.h"
#include "stor.h"
#include "status.h"

#if defined(__cplusplus)

SimpleStatus al_compress(enum ALGreenleafCompressionLevels compression_level,
                         ALStorage &input, ALStorage &output);

SimpleStatus al_decompress(enum ALGreenleafCompressionLevels compression_level,
                           ALStorage &input, ALStorage &output,
                           long compressed_length = -1);

#endif
#endif
