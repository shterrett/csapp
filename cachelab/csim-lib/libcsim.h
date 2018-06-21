#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int TRACE_FILE_NAME_LENGTH;

typedef struct CacheConfig {
  int set_idx_bits;
  int lines_per_set;
  int block_bits;
} cache_config_t;

int init(int argc,
         char **argv,
         int *verbose,
         cache_config_t *config,
         char *trace_file
        );
void print_help(void);

unsigned set_idx(unsigned addr, int n_set_bits);
