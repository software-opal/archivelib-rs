#ifndef NEW__EXPAND_HPP
#define NEW__EXPAND_HPP

#include "new/const.hpp"
#include "new/expand_struct.hpp"
// #include "support/expand.hpp"

ALErrors create_expand_data(RExpandData *data, ALStorage &in_storage,
                            ALStorage &out_storage, size_t _264, int32_t _234);

void free_expand_data(RExpandData *data);
void reset_expand_data(RExpandData *data);

#endif
