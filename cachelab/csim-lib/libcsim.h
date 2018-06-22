#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int TRACE_FILE_NAME_LENGTH;

typedef struct CacheConfig {
  int set_idx_bits;
  int lines_per_set;
  int block_bits;
} cache_config_t;

typedef struct CacheLine {
  uint64_t tag;
  int valid;
  void *block;
  struct CacheLine *prev;
  struct CacheLine *next;
} cache_line_t;

typedef cache_line_t *cache_t;

int init(int argc,
         char **argv,
         int *verbose,
         cache_config_t *config,
         char *trace_file
        );

uint64_t extract_set_idx(cache_config_t *config, uint64_t addr);
uint64_t extract_tag(cache_config_t *config, uint64_t addr);

uint64_t calculate_total_line_space(cache_config_t *config);
uint64_t calculate_total_block_space(cache_config_t *config);

cache_t initialize_cache(cache_config_t *config, cache_line_t *line_array);

typedef enum CACHE_RESULT {
  HIT,
  MISS,
  EVICTION
} cache_result_t;

cache_result_t access_cache(cache_config_t *config, cache_t cache, uint64_t addr);
