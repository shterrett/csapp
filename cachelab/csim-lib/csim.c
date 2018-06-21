#include <stdlib.h>
#include <stdio.h>
#include "cachelab.h"
#include "libcsim.h"

int main(int argc, char** argv) {
  int verbose = 0;
  cache_config_t *config = malloc(sizeof(cache_config_t));
  char* trace_file = malloc(sizeof(char) * TRACE_FILE_NAME_LENGTH);

  int args = init(argc,
                  argv,
                  &verbose,
                  config,
                  trace_file
                 );
  if (args % 2) {
    print_help();
  } else if (args < 8) {
    print_help();
  } else {
    printf("initialized %d %d %d %d %s\n",
          verbose,
          config->set_idx_bits,
          config->lines_per_set,
          config->block_bits,
          trace_file
        );
  };

  return 0;
}

void print_help(void) {
  char *help = "" \
"Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>    \n" \
"Options:                                                    \n" \
"  -h         Print this help message.                       \n" \
"  -v         Optional verbose flag.                         \n" \
"  -s <num>   Number of set index bits.                      \n" \
"  -E <num>   Number of lines per set.                       \n" \
"  -b <num>   Number of block offset bits.                   \n" \
"  -t <file>  Trace file.                                    \n" \
"                                                            \n" \
"Examples:                                                   \n" \
"  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace      \n" \
"  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace   \n";

  printf("%s", help);
  exit(0);
}
