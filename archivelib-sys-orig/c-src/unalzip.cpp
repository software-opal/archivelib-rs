#include "api.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>


int main() {
  uint8_t *input_buffer = (uint8_t*) calloc(sizeof(uint8_t), 65535);

  size_t input_size = fread(input_buffer, sizeof(uint8_t), 65535, stdin);

  AllocatedMemory result = compress(input_buffer, input_size, 4);
  free(input_buffer);
  int16_t status = result.status;

  if (status) {
    fwrite(result.data, sizeof(uint8_t), result.length, stderr);
  } else {
    fwrite(result.data, sizeof(uint8_t), result.length, stdout);
  }
  clean(&result);

  return status == 0 ? 0 : 1;
}
