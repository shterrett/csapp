#include "cachelab.h"
#include "libcsim.h"

void print_help(void);
void simulate_cache(cache_config_t *config, cache_t cache, char *trace_file, int verbose);
void verbose_log(line_t *line, cache_result_t result);

int main(int argc, char** argv) {

  printf("_POSIX_C_SOURCE %ld\n", _POSIX_C_SOURCE);

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

  cache_line_t *lines = malloc(calculate_total_line_space(config));
  cache_t cache = initialize_cache(config, lines);

  simulate_cache(config, cache, trace_file, verbose);

  free(lines);
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

void simulate_cache(cache_config_t *config, cache_t cache, char *trace_file, int verbose) {
  FILE *fp = fopen(trace_file, "r");
  size_t read = 0;
  char *line_str = NULL;
  size_t len = 0;
  line_t *line = malloc(sizeof(line_t));
  counter_t *counter = malloc(sizeof(counter));
  counter->hits = 0;
  counter->misses = 0;
  counter->evictions = 0;

  while ((read = getline(&line_str, &len, fp)) != -1) {
    parse_line(line_str, line);
    cache_result_t result = simulate_cache_access(config, cache, line, counter);

    if (verbose) {
      verbose_log(line, result);
    }
  }

  printSummary(counter->hits, counter->misses, counter->evictions);
  free(counter);
  free(line);
}

void verbose_log(line_t *line, cache_result_t result) {
  switch (line->command) {
    case INSTR:
      break;
    case LOAD:
      printf("L: ");
      break;
    case STORE:
      printf("S: ");
      break;
    case MODIFY:
      printf("M: ");
      break;
    default:
      return;
  }

  printf("0x%016lx ", line->addr);

  switch(result) {
    case HIT:
      printf("hit");
      break;
    case MISS:
      printf("miss");
      break;
    case EVICTION:
      printf("miss, evict");
      break;
    default:
      break;
  }

  if (line->command == MODIFY) {
    printf(", hit");
  }
}
