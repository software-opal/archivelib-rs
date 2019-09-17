#ifndef NEW__COMPRESS_HPP
#define NEW__COMPRESS_HPP

#include "aldefs.h"
#include "new/compress_struct.h"
#include "new/const.h"
#include "support/compress.h"

#ifdef __cplusplus
extern "C" {
#endif

ALErrors create_compress_data(RCompressData *data, ALStorage *in_storage,
                              size_t length, ALStorage *out_storage,
                              ALGreenleafCompressionLevels compression_level,
                              bool fail_uncompressible);

void free_compress_data(RCompressData *data);
void reset_compress_data(RCompressData *data);

void flush_to_output(RCompressData *data);
void calculate_pointer_depths(uint16_t *left_array_ptr,
                              uint16_t *right_array_ptr,
                              uint16_t *depth_store_ptr, uint16_t depth,
                              int16_t series_start, uint16_t curr_idx);

#ifdef __cplusplus
}
#endif

#endif
