#include "support/expand.h"

#include "r_expand.hpp"

void fn258(RExpandData *data, int32_t arg_arr260_len, uint8_t *arg_arr260,
           int32_t bit_size261, uint16_t *output_table262,
           uint16_t max_internal263) {
  DE;
  // AL_ASSERT(max_internal263 == (1 << bit_size261), "");
  uint16_t _277[17], lookup_table287[17], lookup_table288[18], *_204;
  uint32_t i, _289, item209, j, rem_bit_size291, _292, tmp293, _283;

  memset(_277, 0, 17 * sizeof(uint16_t));
  memset(lookup_table287, 0, 17 * sizeof(uint16_t));
  memset(lookup_table288, 0, 18 * sizeof(uint16_t));

  for (i = 0; i < arg_arr260_len; i++) {
    _277[arg_arr260[i]]++;
  }
  for (i = 1; i < 17; i++) {
    // This wraps around to 0.
    lookup_table288[i + 1] = lookup_table288[i] + (_277[i] << (16 - i));
  }
  if (lookup_table288[17] != 0) {
    printf("Lookup table wrong\n");
    data->error = -101;
    // mStatus.SetError(AL_INTERNAL_ERROR, INTERNAL_ERROR_1_MSG);
    data->error_counter243 = 10;
    return;
  }
  rem_bit_size291 = 16 - bit_size261;
  for (i = 1; i <= bit_size261; i++) {
    lookup_table288[i] >>= rem_bit_size291;
    lookup_table287[i] = (uint16_t)(1U << (bit_size261 - i));
  }
  for (; i <= 16; i++) {
    lookup_table287[i] = (uint16_t)(1U << (16 - i));
  }
  i = lookup_table288[bit_size261 + 1] >> rem_bit_size291;
  if (i != (uint16_t)(1U << 16)) {
    _289 = 1U << bit_size261;
    while (i != _289) {
      output_table262[i++] = 0;
    }
  }
  _292 = arg_arr260_len;
  _283 = 1U << (15 - bit_size261);

  for (j = 0; j < arg_arr260_len; j++) {
    item209 = arg_arr260[j];
    if (item209 == 0) {
      continue;
    }
    tmp293 = lookup_table288[item209] + lookup_table287[item209];
    if (item209 <= bit_size261) {
      if (tmp293 > max_internal263) {
        printf("Max internal wrong\n");
        data->error = -102;
        // mStatus.SetError(AL_INTERNAL_ERROR, INTERNAL_ERROR_2_MSG);
        data->error_counter243 = 10;
        return;
      }
      for (i = lookup_table288[item209]; i < tmp293; i++) {
        output_table262[i] = (uint16_t)j;
      }
    } else {
      _289 = lookup_table288[item209];
      _204 = &output_table262[_289 >> rem_bit_size291];
      i = item209 - bit_size261;
      while (i != 0) {
        if (*_204 == 0) {
          data->dat_arr190[_292] = data->dat_arr189[_292] = 0;
          *_204 = (uint16_t)_292++;
        }
        if (_289 & _283) {
          _204 = &data->dat_arr190[*_204];
        } else {
          _204 = &data->dat_arr189[*_204];
        }
        _289 <<= 1;
        i--;
      }
      *_204 = (uint16_t)j;
    }
    lookup_table288[item209] = (uint16_t)tmp293;
  }
  DE;
}
