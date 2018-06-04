#include <stdlib.h>
#include <stdio.h>
#include "cachelab.h"
#include "libcsim.h"

int main(int argc, char** argv) {
  int verbose = 0;
  int set_idx_bits = 0;
  int lines_per_set = 0;
  int block_bits = 0;
  char* trace_file = malloc(sizeof(char) * 32);

  init(argc, argv, &set_idx_bits, &lines_per_set, &block_bits, trace_file);

  printf("initialized %d %d %d %d %s\n",
         verbose,
         set_idx_bits,
         lines_per_set,
         block_bits,
         trace_file
      );

  return 0;
}
