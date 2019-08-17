#ifndef R_EXPAND_HPP
#define R_EXPAND_HPP

#include "new/expand.h"

#ifdef __cplusplus
extern "C" {
#endif

// # define read_bits(DATA, BITS_TO_LOAD) read_bits__(DATA, BITS_TO_LOAD,
// __FILE__, __LINE__) # define get_bits(DATA, BITS_TO_LOAD) get_bits__(DATA,
// BITS_TO_LOAD, __FILE__, __LINE__) uint16_t get_bits__(RExpandData *data,
// uint8_t bits_to_load219, const char* file, size_t line); void
// read_bits__(RExpandData *data, int32_t bits_to_load219, const char* file,
// size_t line);
uint16_t get_bits(RExpandData *data, uint8_t bits_to_load219);
void read_bits(RExpandData *data, int32_t bits_to_load219);

uint16_t get_next_item(RExpandData *data);
uint16_t calculate_run_offset(RExpandData *data);
void fn251(RExpandData *data);
void fn253(RExpandData *data, int16_t _254, int16_t _220, int16_t _221);
void fn255(RExpandData *data);
void fn257(RExpandData *data);
void fn258(RExpandData *data, int32_t _259, uint8_t *_260, int32_t _261,
           uint16_t *_262, uint16_t _263);

int32_t Expand(RExpandData *data);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class RExpand {
public:
  RExpandData *data;

  RExpand(ALStorage &_233, ALStorage &_202, ssize_t _264, int32_t _234);
  ~RExpand();
  ALStatus mStatus;

protected:
  RExpand(RExpand &);
  RExpand &operator=(RExpand &);
};

#endif
#endif
