#ifndef NEW__EXPAND_HPP
#define NEW__EXPAND_HPP

#include "new/const.h"
#include "new/expand_struct.h"
#include "aldefs.h"

#ifdef __cplusplus
extern "C" {
#endif

void __read_buffer(RExpandData *data, size_t length);

ALErrors create_expand_data(RExpandData *data, ALStorage *in_storage,
                            ALStorage *out_storage, ssize_t in_length,
                            int compression_level);

void free_expand_data(RExpandData *data);
void reset_expand_data(RExpandData *data);

void seed_expand(RExpandData *data);

void expand_read_bits(RExpandData *data, uint8_t bits_to_load219);
uint16_t expand_get_bits(RExpandData *data, uint8_t bits_to_load219);

#ifdef __cplusplus
}
#endif

#endif
