#ifndef _R_DATA_HPP
#define _R_DATA_HPP

#ifndef ARRAY_CONTENT_DEBUG
#define WRITE_ARRAY_CONTENT(stream, arr, len)                                  \
  do {                                                                         \
    (stream) << "[";                                                           \
    SET_HEX(stream);                                                           \
    for (size_t i = 0; i < len; i++) {                                         \
      (stream) << (i == 0 ? "" : ", ") << ((int64_t)arr[i]);                   \
    }                                                                          \
    (stream) << "]";                                                           \
    UNSET_HEX(stream);                                                         \
  } while (0);
#else
#define WRITE_ARRAY_CONTENT(stream, arr, len) (stream) << "\"...\"";
#endif

#define SET_HEX(stream)                                                        \
  (stream).setf(std::ios::hex, std::ios::basefield);                           \
  (stream).setf(std::ios::showbase);

#define UNSET_HEX(stream)                                                      \
  (stream).unsetf(std::ios::hex);                                              \
  (stream).unsetf(std::ios::showbase);

#define WRITE_DATA_ARRAY(stream, data, arr, type)                              \
  WRITE_ARRAY(stream, #arr, (data)->arr, type, (data)->arr##_len)

#define WRITE_ARRAY(stream, name, arr, arr_type, len)                          \
  (stream) << ", \"" << (name) << "\": ";                                      \
  _WRITE_ARRAY(stream, arr, arr_type, len)

#define WRITE_ARRAY_PTR(stream, data, name, ptr, arr_type)                     \
  (stream) << ", \"" << (name) << "\": ";                                      \
  (stream) << "{\"type\": \"" << #arr_type << "\"";                            \
  if ((ptr) == NULL) {                                                         \
  }                                                                            \
  _ARRAY_PTR_COND_##arr_type(stream, data, ptr);                               \
  SET_HEX(stream);                                                             \
  (stream) << ", \"startPtr\": " << (intptr_t)(ptr);                           \
  UNSET_HEX(stream);                                                           \
  stream << "}";

#define WRITE_DATA_ARRAY_PTR(stream, data, arr, arr_type)                      \
  WRITE_ARRAY_PTR(stream, data, #arr, arr, arr_type)

#define WRITE_STORAGE(stream, data, name)                                      \
  stream << ", \"" #name "\": ";                                               \
  stream << "{\"position\": " << (data)->name->Tell();                         \
  stream << ", \"size\": " << (data)->name->GetSize();                         \
  stream << "}";

#define WRITE_DATA_HEX(stream, data, name)                                          \
  WRITE_HEX(stream, #name, (data)->name)

#define WRITE_DATA_DEC(stream, data, name)          \
  WRITE_DEC(stream, #name, (data)->name)

#define WRITE_DATA_BOOL(stream, data, name)    \
  WRITE_BOOL(stream, #name, (data)->name)

#define WRITE_HEX(stream, name, value) \
  SET_HEX(stream);                                                             \
  stream << ", \"" << (name) << "\": " << (intmax_t)(value);                               \
  UNSET_HEX(stream);

#define WRITE_DEC(stream, name, value)          \
  stream << ", \"" << (name) << "\": " << (intmax_t)(value);

#define WRITE_BOOL(stream, name, value) \
  stream << ", \"" << (name) << "\": " << ((value) ? "true" : "false");

/************** INTERNAL MACROS **********************************************/

#define _WRITE_ARRAY(stream, arr, arr_type, len)                               \
  (stream) << "{\"type\": \"" << #arr_type << "\"";                            \
  (stream) << ", \"length\": " << len;                                         \
  SET_HEX(stream);                                                             \
  (stream) << ", \"startPtr\": " << (intptr_t)(arr);                           \
  (stream) << ", \"endPtr\": " << (intptr_t)(&(arr)[len - 1]);                 \
  UNSET_HEX(stream);                                                           \
  (stream) << ", \"content\": ";                                               \
  WRITE_ARRAY_CONTENT(stream, arr, len)                                        \
  stream << "}";

#define _ARRAY_PTR_COND_EXTRA(stream, ptr, arr, start, len)                    \
  (stream) << ", \"len\": \"" << (len) - (start) << "\"";                      \
  (stream) << ", \"content\": ";                                               \
  WRITE_ARRAY_CONTENT(stream, arr, (len) - (start))

#define _ARRAY_PTR_COND(stream, ptr, data, arr)                                \
  else if ((data)->arr <= (ptr) &&                                             \
           (ptr) <= &((data)->arr)[(data)->arr##_len - 1]) {                   \
    (stream) << ", \"in\": \"" #arr "\"";                                      \
    (stream) << ", \"start\": ";                                               \
    (stream) << (ptr - (data)->arr) / sizeof(*(data)->arr);                    \
    _ARRAY_PTR_COND_EXTRA(stream, ptr, data->arr,                              \
                          (ptr - (data)->arr) / sizeof(*(data)->arr),          \
                          (data)->arr##_len)                                   \
  }

#define _ARRAY_PTR_COND_int16_t(stream, data, arr)                             \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr163)                               \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr164)                               \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr177)

#define _ARRAY_PTR_COND_uint16_t(stream, data, arr)                            \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr167)                               \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr189)                               \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr190)                               \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr191)                               \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr192)                               \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr193)                               \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr194)

#define _ARRAY_PTR_COND_uint8_t(stream, data, arr)                             \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr165)                               \
  _ARRAY_PTR_COND(stream, arr, data, input_buffer)                               \
  _ARRAY_PTR_COND(stream, arr, data, buffer)                                   \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr180)                               \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr181)
#endif