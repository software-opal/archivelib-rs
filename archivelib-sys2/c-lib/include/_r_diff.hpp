#include <string>
#include <iostream>
#include <sstream>
#include <string.h>

#define DIFF_ARRAY(stream, has_changes, name, old_array, new_array, length)    \
  {                                                                            \
    char data[1000];                                                           \
    for (size_t idx = 0; idx < length; idx++) {                                \
      if (old_array[idx] != new_array[idx]) {                                  \
        has_changes = true;                                                    \
        sprintf(data, "    | %12s[%6zu] | %10d | %2s | %10d |\n", name, idx,   \
                old_array[idx], "<>", new_array[idx]);                         \
        stream << data;                                                        \
      }                                                                        \
    }                                                                          \
  }
