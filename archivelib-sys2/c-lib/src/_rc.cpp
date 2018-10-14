#include "_rc.hpp"
#include <stdlib.h>
#include <cstring>

#define MACRO_FN445(arg278, arg200, arg446)                                    \
  ((int16_t)((arg446 << CONST__154) ^ (arg278[arg200 + 2])) & (CONST__153 - 1))
#define MACRO_FN447(arg163, arg164, arg200, arg201)                            \
  {                                                                            \
    int16_t macro_local204;                                                    \
    if ((macro_local204 = arg163[arg201]) != CONST__157)                       \
      arg164[macro_local204] = arg200;                                         \
    arg164[arg200] = arg201;                                                   \
    arg163[arg200] = macro_local204;                                           \
    arg163[arg201] = arg200;                                                   \
  }
#define MACRO_FN448(arg163, arg164, s)                                         \
  {                                                                            \
    int16_t macro_local204;                                                    \
    if ((macro_local204 = arg164[s]) != CONST__157) {                          \
      arg164[s] = CONST__157;                                                  \
      arg163[macro_local204] = CONST__157;                                     \
    }                                                                          \
  }
RCompress::RCompress(ALStorage &in_storage, ALStorage &out_storage,
                     ALGreenleafCompressionLevels compression_level,
                     bool fail_uncompressible) {
  data = (RCompressData *)calloc(1, sizeof(RCompressData));
  ALErrors res = create_compress_data(data, in_storage, out_storage,
                                      compression_level, fail_uncompressible);
  switch (res) {
  case AL_SUCCESS:
    break;
  case AL_ILLEGAL_PARAMETER:
    mStatus.SetError(AL_ILLEGAL_PARAMETER, _519, compression_level);
    break;
  case AL_CANT_ALLOCATE_MEMORY:
    mStatus.SetError(AL_CANT_ALLOCATE_MEMORY, _520);
    break;
  default:
    mStatus.SetError(res, "Other Error");
  }
}
RCompress::~RCompress() {
  if (data) {
    free_compress_data(data);
    free(data);
    data = NULL;
  }
}
void RCompress::fn223(int16_t _203) {
  fn208(data->dat_arr180[_203], data->dat_arr192[_203]);
}
int32_t RCompress::Compress() {
  int16_t local209;
  int16_t local201;
  int16_t local200;
  int16_t s;
  int32_t local231;
  uint8_t *local278;
  int16_t local280;
  int16_t local279;
  local278 = data->dat_arr166;
  local280 = data->compression_level_bit_minus_one;
  local279 = data->compression_level_bit;
  local231 = 0;
  fn196();
  fn198();
  local200 = 0;
  local209 = (int16_t)data->input_store->ReadBuffer(local278, local279);
  s = (int16_t)(local209 & local280);
  data->dat169 = 0;
  data->dat168 = 0;
  local201 = (int16_t)(
      ((local278[local200] << CONST__154) ^ (local278[local200 + 1])) &
      (CONST__153 - 1));
  local201 = (int16_t)(MACRO_FN445(local278, local200, local201) + local279);
  while (local209 > CONST__140 + 4 && !data->uncompressible) {
    fn199(local200, local201);
    if (data->dat168 < CONST__135) {
      fn202(local278[local200], 0);
      MACRO_FN447(data->dat_arr163, data->dat_arr164, local200, local201);
      local200++;
      local201 =
          (int16_t)(MACRO_FN445(local278, local200, local201) + local279);
      local209--;
    } else {
      local209 -= data->dat168;
      fn202((uint16_t)(data->dat168 + (UCHAR_MAX + 1 - CONST__135)),
            data->dat169);
      while (--data->dat168 >= 0) {
        MACRO_FN447(data->dat_arr163, data->dat_arr164, local200, local201);
        local200++;
        local201 =
            (int16_t)(MACRO_FN445(local278, local200, local201) + local279);
      }
    }
  }
  for (; local209 < CONST__140; local209++) {
    int32_t _203 = data->input_store->ReadChar();
    if (_203 < 0)
      break;
    local278[s] = (unsigned char)_203;
    if (s < CONST__140 - 1)
      local278[s + local279] = local278[s];
    MACRO_FN448(data->dat_arr163, data->dat_arr164, s);
    s = (int16_t)((s + 1) & (local280));
  }
  while (local209 > 0 && !data->uncompressible) {
    fn199(local200, local201);
    if (data->dat168 > local209)
      data->dat168 = local209;
    if (data->dat168 < CONST__135) {
      data->dat168 = 1;
      fn202(local278[local200], 0);
    } else
      fn202((uint16_t)(data->dat168 + (UCHAR_MAX + 1 - CONST__135)),
            data->dat169);
    while (--data->dat168 >= 0) {
      int32_t _203 = data->input_store->ReadChar();
      if (_203 < 0)
        break;
      else
        local278[s] = (unsigned char)_203;
      if (s < CONST__140 - 1)
        local278[s + local279] = local278[s];
      MACRO_FN448(data->dat_arr163, data->dat_arr164, s);
      s = (int16_t)((s + 1) & (local280));
      MACRO_FN447(data->dat_arr163, data->dat_arr164, local200, local201);
      local200 = (int16_t)((local200 + 1) & (local280));
      local201 =
          (int16_t)(MACRO_FN445(local278, local200, local201) + local279);
    }
    while (data->dat168-- >= 0) {
      MACRO_FN447(data->dat_arr163, data->dat_arr164, local200, local201);
      local200 = (int16_t)((local200 + 1) & local280);
      local201 =
          (int16_t)(MACRO_FN445(local278, local200, local201) + local279);
      local209--;
    }
    if (data->output_store->mStatus < 0)
      return 1;
  }
  if (!data->uncompressible)
    fn202(CONST__144 + (UCHAR_MAX + 1 - CONST__135), 0);
  fn197();
  if (data->uncompressible)
    local231 = 1;
  return local231;
}
void RCompress::fn196() {
  int32_t i;
  for (i = 0; i < CONST__141; i++)
    data->dat_arr191[i] = 0;
  for (i = 0; i < CONST__142; i++)
    data->dat_arr193[i] = 0;
  data->dat173 = 0;
  fn205();
  data->uncompressible = 0;
  data->dat185 = 1;
  data->dat184 = 0;
  data->dat186 = 0;
  data->dat_arr165[0] = 0;
  data->dat183 = CONST__155;
  data->dat183 -= (uint16_t)((3 * CHAR_BIT) + 6);
}
void RCompress::fn197() {
  if (!data->uncompressible)
    fn207();
  fn206();
  data->dat183 = 0;
  data->dat184 = 0;
}
void RCompress::fn198() {
  int16_t *local450;
  int16_t i;
  local450 = &data->dat_arr163[data->compression_level_bit];
  for (i = CONST__153; i > 0; i--)
    *local450++ = CONST__157;
  local450 = data->dat_arr164;
  for (i = data->compression_level_bit; i > 0; i--)
    *local450++ = CONST__157;
}
void RCompress::fn199(int16_t arg200, int16_t arg201) {
  uint8_t *local451;
  uint8_t *local278;
  int16_t i, local452, local204, local453;
  local452 = CONST__158;
  data->dat168 = 0;
  local451 = &data->dat_arr166[arg200];
  local204 = arg201;
  while ((local204 = data->dat_arr163[local204]) != CONST__157) {
    if (--local452 < 0)
      break;
    local278 = &data->dat_arr166[local204];
    if (local451[data->dat168] != local278[data->dat168])
      continue;
    if (local451[0] != local278[0])
      continue;
    if (local451[1] != local278[1])
      continue;
    if (local451[2] != local278[2])
      continue;
    for (i = 3; i < CONST__140; i++)
      if (local451[i] != local278[i])
        break;
    if (i > data->dat168) {
      local453 = (int16_t)(arg200 - local204 - 1);
      if (local453 < 0)
        local453 += data->compression_level_bit;
      if (local453 >= data->compression_level_bit) {
        break;
      }
      data->dat169 = local453;
      if ((data->dat168 = i) >= CONST__140)
        break;
    }
  }
}
void RCompress::fn202(uint16_t arg203, uint16_t arg204) {
  if ((data->dat185 >>= 1) == 0) {
    data->dat185 = 1U << (CHAR_BIT - 1);
    if (data->dat184 >= data->dat183) {
      fn207();
      if (data->uncompressible)
        return;
      data->dat184 = 0;
    }
    data->dat186 = data->dat184++;
    data->dat_arr165[data->dat186] = 0;
  }
  data->dat_arr165[data->dat184++] = (uint8_t)arg203;
  data->dat_arr191[arg203]++;
  if (arg203 >= (1U << CHAR_BIT)) {
    data->dat_arr165[data->dat186] |= (uint8_t)data->dat185;
    data->dat_arr165[data->dat184++] = (uint8_t)arg204;
    data->dat_arr165[data->dat184++] = (uint8_t)(arg204 >> CHAR_BIT);
    arg203 = 0;
    while (arg204) {
      arg203++;
      arg204 >>= 1;
    }
    data->dat_arr193[arg203]++;
  }
}
void RCompress::fn205() {
  data->dat172 = 0;
  data->dat182 = 0;
  data->dat171 = 0;
}
void RCompress::fn206() {
  if (!data->uncompressible) {
    fn208(CHAR_BIT - 1, 0);
    if (data->dat171)
      flush_to_output();
  }
  data->dat171 = 0;
}
void RCompress::fn207() {
  uint32_t i, local289, local229, local454, local455;
  uint32_t local456 = 0;
  uint16_t local217[2 * CONST__145 - 1];
  local229 =
      fn211(CONST__141, data->dat_arr191, data->dat_arr180, data->dat_arr192);
  local455 = data->dat_arr191[local229];
  fn208(16, (uint16_t)local455);
  if (local229 >= CONST__141) {
    fn216(local217);
    local229 = fn211(CONST__145, local217, data->dat_arr181, data->dat_arr194);
    if (local229 >= CONST__145) {
      fn218(CONST__145, CONST__147, 3);
    } else {
      fn208(CONST__147, 0);
      fn208(CONST__147, (uint16_t)local229);
    }
    fn222();
  } else {
    fn208(CONST__147, 0);
    fn208(CONST__147, 0);
    fn208(CONST__143, 0);
    fn208(CONST__143, (uint16_t)local229);
  }
  local229 =
      fn211(CONST__142, data->dat_arr193, data->dat_arr181, data->dat_arr194);
  if (local229 >= CONST__142) {
    fn218(CONST__142, CONST__540, -1);
  } else {
    fn208(CONST__540, 0);
    fn208(CONST__540, (uint16_t)local229);
  }
  local454 = 0;
  for (i = 0; i < local455; i++) {
    if (i % CHAR_BIT == 0)
      local456 = data->dat_arr165[local454++];
    else
      local456 <<= 1;
    if (local456 & (1U << (CHAR_BIT - 1))) {
      fn223((int16_t)(data->dat_arr165[local454++] + (1U << CHAR_BIT)));
      local289 = data->dat_arr165[local454++];
      local289 += data->dat_arr165[local454++] << CHAR_BIT;
      fn224((int16_t)local289);
    } else
      fn223(data->dat_arr165[local454++]);
    if (data->uncompressible)
      return;
  }
  for (i = 0; i < CONST__141; i++)
    data->dat_arr191[i] = 0;
  for (i = 0; i < CONST__142; i++)
    data->dat_arr193[i] = 0;
}
void RCompress::fn208(int32_t arg209, uint16_t arg203) {
  arg203 <<= CONST__133 - arg209;
  data->dat182 |= (uint16_t)(arg203 >> data->dat172);
  if ((data->dat172 += (int16_t)arg209) >= 8) {
    if (data->dat171 >= CONST__156)
      flush_to_output();
    data->dat_arr179[data->dat171++] = (uint8_t)(data->dat182 >> CHAR_BIT);
    if ((data->dat172 = (uint16_t)(data->dat172 - CHAR_BIT)) < CHAR_BIT)
      data->dat182 <<= CHAR_BIT;
    else {
      if (data->dat171 >= CONST__156)
        flush_to_output();
      data->dat_arr179[data->dat171++] = (uint8_t)data->dat182;
      data->dat172 = (uint16_t)(data->dat172 - CHAR_BIT);
      data->dat182 = (uint16_t)(arg203 << (arg209 - data->dat172));
    }
  }
}
void RCompress::flush_to_output() {
  if (data->dat171 <= 0)
    return;
  if (data->fail_uncompressible &&
      (data->chars_written += data->dat171) >= data->input_length)
    data->uncompressible = 1;
  else
    data->output_store->WriteBuffer(data->dat_arr179, data->dat171);
  data->dat171 = 0;
}
int32_t RCompress::fn211(int32_t arg212, uint16_t *arg213, uint8_t *arg214,
                         uint16_t *arg215) {
  int32_t i, local276, local289, local292;
  int16_t local227;
  data->dat174 = (int16_t)arg212;
  data->dat_arr187 = arg213;
  data->dat_arr178 = arg214;
  local292 = data->dat174;
  local227 = 0;
  data->dat_arr177[1] = 0;
  for (i = 0; i < data->dat174; i++) {
    data->dat_arr178[i] = 0;
    if (data->dat_arr187[i])
      data->dat_arr177[++local227] = (int16_t)i;
  }
  if (local227 < 2) {
    arg215[data->dat_arr177[1]] = 0;
    return data->dat_arr177[1];
  }
  for (i = local227 / 2; i >= 1; i--)
    fn225(i, data->dat_arr187, data->dat_arr177, local227);
  data->dat_arr188 = arg215;
  do {
    i = data->dat_arr177[1];
    if (i < data->dat174)
      *data->dat_arr188++ = (uint16_t)i;
    data->dat_arr177[1] = data->dat_arr177[local227--];
    fn225(1, data->dat_arr187, data->dat_arr177, local227);
    local276 = data->dat_arr177[1];
    if (local276 < data->dat174)
      *data->dat_arr188++ = (uint16_t)local276;
    local289 = local292++;
    data->dat_arr187[local289] =
        (uint16_t)(data->dat_arr187[i] + data->dat_arr187[local276]);
    data->dat_arr177[1] = (int16_t)local289;
    fn225(1, data->dat_arr187, data->dat_arr177, local227);
    data->dat_arr189[local289] = (uint16_t)i;
    data->dat_arr190[local289] = (uint16_t)local276;
  } while (local227 > 1);
  data->dat_arr188 = arg215;
  fn228(local289);
  fn230(arg212, arg214, arg215);
  return local289;
}
void RCompress::fn216(uint16_t *arg217) {
  int16_t i, local289, local219, local277;
  for (i = 0; i < CONST__145; i++)
    arg217[i] = 0;
  local219 = CONST__141;
  while (local219 > 0 && data->dat_arr180[local219 - 1] == 0)
    local219--;
  i = 0;
  while (i < local219) {
    local289 = data->dat_arr180[i++];
    if (local289 == 0) {
      local277 = 1;
      while (i < local219 && data->dat_arr180[i] == 0) {
        i++;
        local277++;
      }
      if (local277 <= 2)
        arg217[0] += local277;
      else if (local277 <= 18)
        arg217[1]++;
      else if (local277 == 19) {
        arg217[0]++;
        arg217[1]++;
      } else
        arg217[2]++;
    } else
      arg217[local289 + 2]++;
  }
}
void RCompress::fn218(int16_t arg219, int16_t arg220, int16_t arg221) {
  int16_t i, local289;
  while (arg219 > 0 && data->dat_arr181[arg219 - 1] == 0)
    arg219--;
  fn208(arg220, arg219);
  i = 0;
  while (i < arg219) {
    local289 = data->dat_arr181[i++];
    if (local289 <= 6) {
      fn208(3, local289);
    } else
      fn208(local289 - 3, (uint16_t)(USHRT_MAX << 1));
    if (i == arg221) {
      while (i < 6 && data->dat_arr181[i] == 0)
        i++;
      fn208(2, (uint16_t)(i - 3));
    }
  }
}
void RCompress::fn222() {
  int16_t i, local289, local219, local277;
  local219 = CONST__141;
  while (local219 > 0 && data->dat_arr180[local219 - 1] == 0)
    local219--;
  fn208(CONST__143, local219);
  i = 0;
  while (i < local219) {
    local289 = data->dat_arr180[i++];
    if (local289 == 0) {
      local277 = 1;
      while (i < local219 && data->dat_arr180[i] == 0) {
        i++;
        local277++;
      }
      if (local277 <= 2) {
        for (local289 = 0; local289 < local277; local289++)
          fn208(data->dat_arr181[0], data->dat_arr194[0]);
      } else if (local277 <= 18) {
        fn208(data->dat_arr181[1], data->dat_arr194[1]);
        fn208(4, (uint16_t)(local277 - 3));
      } else if (local277 == 19) {
        fn208(data->dat_arr181[0], data->dat_arr194[0]);
        fn208(data->dat_arr181[1], data->dat_arr194[1]);
        fn208(4, 15);
      } else {
        fn208(data->dat_arr181[2], data->dat_arr194[2]);
        fn208(CONST__143, (uint16_t)(local277 - 20));
      }
    } else
      fn208(data->dat_arr181[local289 + 2], data->dat_arr194[local289 + 2]);
  }
}
void RCompress::fn224(uint16_t arg204) {
  uint16_t local203, local457;
  local203 = 0;
  local457 = arg204;
  while (local457) {
    local203++;
    local457 >>= 1;
  }
  fn208(data->dat_arr181[local203], data->dat_arr194[local203]);
  if (local203 > 1)
    fn208(local203 - 1, arg204);
}
void RCompress::fn225(int32_t i, uint16_t *arg187, int16_t *arg177,
                      int16_t arg227) {
  int32_t local276, local289;
  local289 = arg177[i];
  while ((local276 = 2 * i) <= arg227) {
    if (local276 < arg227 &&
        arg187[arg177[local276]] > arg187[arg177[local276 + 1]])
      local276++;
    if (arg187[local289] <= arg187[arg177[local276]])
      break;
    arg177[i] = arg177[local276];
    i = local276;
  }
  arg177[i] = (uint16_t)local289;
}
void RCompress::fn228(int32_t arg229) {
  int32_t i, local289;
  uint32_t local458;
  for (i = 0; i <= 16; i++)
    data->dat_arr167[i] = 0;
  fn232(arg229);
  local458 = 0;
  for (i = 16; i > 0; i--)
    local458 += data->dat_arr167[i] << (16 - i);
  while (local458 != (1U << 16)) {
    data->dat_arr167[16]--;
    for (i = 15; i > 0; i--) {
      if (data->dat_arr167[i] != 0) {
        data->dat_arr167[i]--;
        data->dat_arr167[i + 1] = (uint16_t)(data->dat_arr167[i + 1] + 2);
        break;
      }
    }
    local458--;
  }
  for (i = 16; i > 0; i--) {
    local289 = data->dat_arr167[i];
    while (--local289 >= 0)
      data->dat_arr178[*data->dat_arr188++] = (uint8_t)i;
  }
}
void RCompress::fn230(int32_t arg219, uint8_t *arg209, uint16_t *arg231) {
  int32_t i;
  uint16_t local288[18];
  local288[1] = 0;
  for (i = 1; i <= 16; i++)
    local288[i + 1] = (uint16_t)((local288[i] + data->dat_arr167[i]) << 1);
  for (i = 0; i < arg219; i++)
    arg231[i] = local288[arg209[i]]++;
}

void RCompress::fn232(int32_t arg226) {
  if (arg226 < data->dat174)
    data->dat_arr167[(data->dat173 < 16) ? data->dat173 : 16]++;
  else {
    data->dat173++;
    fn232(data->dat_arr189[arg226]);
    fn232(data->dat_arr190[arg226]);
    data->dat173--;
  }
}
