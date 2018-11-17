
#include "new/compress.h"

void calculate_pointer_depths(uint16_t *left_array_ptr,
                              uint16_t *right_array_ptr,
                              uint16_t *depth_store_ptr, uint16_t depth,
                              int16_t series_start, uint16_t curr_idx) {
  /*
   * Pointer depth calculation?

   * `left_array_ptr` & `right_array_ptr` contain a series(from `series_start`
   to `curr_idx`) of integers that are `< curr_idx`. If they are between
   `series_start` and `curr_idx`, then it's a pointer to another array index.
   Otherwise it's not. This function calculates the number of non-pointer values
   at each depth by following the pointers until a non-pointer, then
   incrementing the count of depth by 1.

   * Note that the pointers will link to the index of both arrays, and need to
   be explored in both arrays. Each value is unique and there are no loops.

   * Does `left_array_ptr` and `right_array_ptr` represent a binary tree?
   */
  if (curr_idx < series_start) {
    if (depth < 16) {
      depth_store_ptr[depth]++;
    } else {
      depth_store_ptr[16]++;
    }
  } else {
    calculate_pointer_depths(left_array_ptr, right_array_ptr, depth_store_ptr,
                             depth + 1, series_start, left_array_ptr[curr_idx]);
    calculate_pointer_depths(left_array_ptr, right_array_ptr, depth_store_ptr,
                             depth + 1, series_start,
                             right_array_ptr[curr_idx]);
  }
}
