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

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " (-0|-1|-2|-3|-4)" << std::endl;
    return 1;
  }
  char* c_level_opt = argv[1];
  uint8_t level= 255;
  if(strncmp("-0", c_level_opt, 2) == 0) { level = 0; }
  else if(strncmp("-1", c_level_opt, 2) == 0) { level = 1; }
  else if(strncmp("-2", c_level_opt, 2) == 0) { level = 2; }
  else if(strncmp("-3", c_level_opt, 2) == 0) { level = 3; }
  else if(strncmp("-4", c_level_opt, 2) == 0) { level = 4; }
  else {
    std::cerr << "Usage: " << argv[0] << " -0|-1|-2|-3|-4" << std::endl;
    return 1;
  }

  uint8_t *input_buffer = (uint8_t*) calloc(sizeof(uint8_t), 65535);

  size_t input_size = fread(input_buffer, sizeof(uint8_t), 65535, stdin);

  AllocatedMemory result = compress(input_buffer, input_size, level);
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
