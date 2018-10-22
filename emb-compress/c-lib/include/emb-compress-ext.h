#ifndef EMB_COMPRESS_EXT_H
#define EMB_COMPRESS_EXT_H

#include "emb-pattern.h"

#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL husExpand(unsigned char *input,
                                          unsigned char *output,
                                          int compressedSize, int _269);
extern EMB_PRIVATE int EMB_CALL husCompress(unsigned char *_266,
                                            unsigned long _inputSize,
                                            unsigned char *_267, int _269,
                                            int _235);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_COMPRESS_H */

/* kate: bom off; indent-mode cstyle; indent-width 4;
 * replace-trailing-space-save on; */
