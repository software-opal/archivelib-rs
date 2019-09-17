#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>
#include "api.h"

int main() {
  uint8_t *input_buffer = (uint8_t*) calloc(sizeof(uint8_t), 65535);

  size_t input_size = fread(input_buffer, sizeof(uint8_t), 65535, stdin);

  AllocatedMemory2 result = compress2(input_buffer, input_size, 4);
  free(input_buffer);
  int status = result.status;

  if (status) {
    fwrite(result.data, sizeof(uint8_t), result.length, stderr);
  } else {
    fwrite(result.data, sizeof(uint8_t), result.length, stdout);
  }
  clean2(&result);

  return status == 0 ? 0 : 1;
}
