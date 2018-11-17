#ifndef NEW__COMPRESS_HPP
#define NEW__COMPRESS_HPP

#include "new/const.h"
#include "new/compress_struct.h"
#include "support/compress.h"
#include "aldefs.h"

ALErrors create_compress_data(RCompressData *data, ALStorage &in_storage,
                              ALStorage &out_storage,
                              ALGreenleafCompressionLevels compression_level,
                              bool fail_uncompressible);

void free_compress_data(RCompressData *data);
void reset_compress_data(RCompressData *data);

void flush_to_output(RCompressData *data);
void calculate_pointer_depths(uint16_t *left_array_ptr,
                              uint16_t *right_array_ptr,
                              uint16_t *depth_store_ptr, uint16_t depth,
                              int16_t series_start, uint16_t curr_idx);

#endif
