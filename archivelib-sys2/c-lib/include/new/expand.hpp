#ifndef NEW__EXPAND_HPP
#define NEW__EXPAND_HPP

#include "new/const.hpp"
#include "new/expand_struct.hpp"
#include "aldefs.h"
// #include "support/expand.hpp"

ALErrors create_expand_data(RExpandData *data, ALStorage &in_storage,
                            ALStorage &out_storage, ssize_t in_length,
                            int compression_level);

void free_expand_data(RExpandData *data);
void reset_expand_data(RExpandData *data);

void expand_read_bits(RExpandData *data, int32_t bits_to_load219);

#endif
