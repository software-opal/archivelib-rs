#ifndef SUPPORT__DEBUG_HPP
#define SUPPORT__DEBUG_HPP

#ifdef NDEBUG

#else
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>

const time_t PROC_START = time(NULL);

std::string get_as_binary(uintmax_t value, uint8_t max_bits);

#define DEBUG_FILE_HANDLE(fh, uniq)                                            \
  std::ofstream fh;                                                            \
  clock_t now = std::clock();                                                  \
  do {                                                                         \
    std::stringstream filename;                                                \
    filename << "target/data-"                                                 \
             << std::put_time(std::localtime(&PROC_START),                     \
                              "%Y-%m-%dT%H-%M-%S%z");                          \
    SET_HEX(filename);                                                         \
    filename << "-" << (intptr_t)(uniq) << ".yaml";                            \
    UNSET_HEX(filename);                                                       \
    fh.open(filename.str(), std::ofstream::out | std::ofstream::app);          \
                                                                               \
    fh << "\n---\n\"file\": \"" << __FILE__ << "\"\n\"line\": " << __LINE__;   \
    fh << "\n\"func\": \"" << __func__ << "\"\n\"now\": " << now;              \
    fh << "\n\"data\": ";                                                      \
                                                                               \
    std::cout << __FILE__ << ":" << __LINE__ << " -- " << __func__ << "(";     \
    std::cout << now << ")\n";                                                 \
  } while (0);

#define WRITE_OUTPUT_BITS(fh, data, bit_count, bits)                           \
  {                                                                            \
    int32_t bc = bit_count;                                                    \
    uint16_t bt = bits;                                                        \
    clock_t now = std::clock();                                                \
    fh << ",'output[" << fh.tellp() << "]': {";                                \
    fh << "'bit_count': " << bc << ", 'bits': " << bt;                         \
    fh << "}";                                                                 \
    std::cout << ",'output[" << now << "]': {";                                \
    std::cout << "'bit_count': " << bc << ", 'bits': " << bt;                  \
    std::cout << "}\n";                                                        \
    write_bits_to_buffer(data, bc, bt);                                        \
  }

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

#define WRITE_SPARSE_ARRAY_CONTENT(stream, arr, len)                           \
  do {                                                                         \
    (stream) << "{";                                                           \
    for (size_t i = 0; i < len; i++) {                                         \
      if (i == 0 || arr[i]) {                                                  \
        (stream) << (i == 0 ? "" : ", ") << i << ": ";                         \
        SET_HEX(stream);                                                       \
        stream << ((int64_t)arr[i]);                                           \
        UNSET_HEX(stream);                                                     \
      }                                                                        \
    }                                                                          \
    (stream) << "}";                                                           \
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

#define WRITE_DATA_SPARSE_ARRAY(stream, data, arr, type)                       \
  WRITE_SPARSE_ARRAY(stream, #arr, (data)->arr, type, (data)->arr##_len)

#define WRITE_SPARSE_ARRAY(stream, name, arr, arr_type, len)                   \
  (stream) << ", \"" << (name) << "\": ";                                      \
  _WRITE_SPARSE_ARRAY(stream, arr, arr_type, len)

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
  WRITE_ARRAY_PTR(stream, data, #arr, (data)->arr, arr_type)

#define WRITE_STORAGE(stream, data, name)                                      \
  stream << ", \"" #name "\": ";                                               \
  stream << "{\"position\": " << (data)->name->Tell();                         \
  stream << ", \"size\": " << (data)->name->GetSize();                         \
  stream << "}";

#define WRITE_DATA_HEX(stream, data, name)                                     \
  WRITE_HEX(stream, #name, (data)->name)

#define WRITE_DATA_DEC(stream, data, name)                                     \
  WRITE_DEC(stream, #name, (data)->name)

#define WRITE_DATA_BOOL(stream, data, name)                                    \
  WRITE_BOOL(stream, #name, (data)->name)

#define WRITE_HEX(stream, name, value)                                         \
  SET_HEX(stream);                                                             \
  stream << ", \"" << (name) << "\": " << (intmax_t)(value);                   \
  UNSET_HEX(stream);

#define WRITE_BITS(stream, name, value)                                        \
  {                                                                            \
    size_t bits = sizeof(value) * 8;                                           \
    char buff[bits + 1];                                                       \
    buff[bits] = '\0';                                                         \
    for (size_t i = 0; i < sizeof(value) * 8; i++) {                           \
      buff[bits - i - 1] = ((value) & (1 << i)) ? '1' : '0';                   \
    }                                                                          \
    stream << ", \"" << (name) << "\": " << buff;                              \
  }

#define WRITE_DEC(stream, name, value)                                         \
  stream << ", \"" << (name) << "\": " << (intmax_t)(value);

#define WRITE_BOOL(stream, name, value)                                        \
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

#define _WRITE_SPARSE_ARRAY(stream, arr, arr_type, len)                        \
  (stream) << "{\"type\": \"" << #arr_type << "\"";                            \
  (stream) << ", \"length\": " << len;                                         \
  SET_HEX(stream);                                                             \
  (stream) << ", \"startPtr\": " << (intptr_t)(arr);                           \
  (stream) << ", \"endPtr\": " << (intptr_t)(&(arr)[len - 1]);                 \
  UNSET_HEX(stream);                                                           \
  (stream) << ", \"content\": ";                                               \
  WRITE_SPARSE_ARRAY_CONTENT(stream, arr, len)                                 \
  stream << "}";

#define _ARRAY_PTR_COND_EXTRA(stream, ptr, arr, start, len)                    \
  (stream) << ", \"in_len\": \"" << (len) << "\"";                             \
  (stream) << ", \"content\": ";                                               \
  WRITE_ARRAY_CONTENT(stream, arr, len)

#define _ARRAY_PTR_COND(stream, ptr, data, arr)                                \
  else if ((data)->arr <= (ptr) &&                                             \
           (ptr) <= &((data)->arr)[(data)->arr##_len - 1]) {                   \
    (stream) << ", \"in\": \"" #arr "\"";                                      \
    (stream) << ", \"start\": ";                                               \
    (stream) << (ptr - (data)->arr);                                           \
    _ARRAY_PTR_COND_EXTRA(stream, ptr, data->arr, (ptr - (data)->arr),         \
                          (data)->arr##_len)                                   \
  }

#define _ARRAY_PTR_COND_bool(stream, data, arr)                                \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr163)                               \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr164)

#define _ARRAY_PTR_COND_int16_t(stream, data, arr)                             \
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
  _ARRAY_PTR_COND(stream, arr, data, uncompressed_buffer)                      \
  _ARRAY_PTR_COND(stream, arr, data, buffer)                                   \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr180)                               \
  _ARRAY_PTR_COND(stream, arr, data, dat_arr181)

#define DIFF_ARRAY(stream, has_changes, name, old_array, new_array, length)    \
  {                                                                            \
    char __data[1000];                                                         \
    for (size_t idx = 0; idx < length; idx++) {                                \
      if (old_array[idx] != new_array[idx]) {                                  \
        has_changes = true;                                                    \
        sprintf(__data, "    | %32s[%6zu] | %10d | %2s | %10d |\n", name, idx, \
                old_array[idx], "<>", new_array[idx]);                         \
        stream << __data;                                                      \
      }                                                                        \
    }                                                                          \
  }

#define INLINE_DIFF_ARR(stream, has_changes, old_data, new_data, arr_name)     \
  {                                                                            \
    bool has_changes_this_time = false;                                        \
    DIFF_ARRAY(stream, has_changes_this_time, #arr_name, old_data->arr_name,   \
               new_data->arr_name, old_data->arr_name##_len);                  \
    if (!has_changes_this_time) {                                              \
    } else {                                                                   \
      has_changes = true;                                                      \
    }                                                                          \
  }
#define INLINE_DIFF_VAL(stream, has_changes, _spec, old_data, new_data,        \
                        val_name)                                              \
  {                                                                            \
    if (old_data->val_name != new_data->val_name) {                            \
      has_changes = true;                                                      \
      char __data[1000];                                                       \
      sprintf(__data, "    | %40s | %10" _spec " | %2s | %10" _spec " |\n",    \
              #val_name, old_data->val_name, "<>", new_data->val_name);        \
      stream << __data;                                                        \
    } else {                                                                   \
    }                                                                          \
  }

#endif
#endif
