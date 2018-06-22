#include "libcsim.h"

int TRACE_FILE_NAME_LENGTH = 32;

int init(int argc,
         char **argv,
         int *verbose,
         cache_config_t *config,
         char *trace_file
        ) {

  int help = 0;
  int required = 0;

  int i = 0;
  while (i < argc) {
    char* arg = argv[i];
    if (strncmp(arg, "-h", 2) == 0) {
      help = 1;
    } else if (strncmp(arg, "-v", 2) == 0) {
      *verbose = 1;
    } else if (strncmp(arg,  "-s", 2) == 0) {
      config->set_idx_bits = strtol(argv[++i], NULL, 10);
      required += 2;
    } else if (strncmp(arg, "-E", 2) == 0) {
      config->lines_per_set = strtol(argv[++i], NULL, 10);
      required += 2;
    } else if (strncmp(arg, "-b", 2) == 0) {
      config->block_bits = strtol(argv[++i], NULL, 10);
      required += 2;
    } else if (strncmp(arg, "-t", 2) == 0) {
      strncpy(trace_file, argv[++i], TRACE_FILE_NAME_LENGTH);
      required += 2;
    }

    i++;
  }

  return required + help;
}

uint64_t build_tag_mask(cache_config_t *config) {
  uint64_t tag_mask = (0x1l << (config->set_idx_bits + config->block_bits)) - 1;
   return tag_mask << (config->set_idx_bits + config->block_bits);
}

uint64_t extract_set_idx(cache_config_t *config, uint64_t addr) {
  uint64_t tag_mask = ~(build_tag_mask(config));
  uint64_t block_mask = ((0x1l << config->set_idx_bits) - 1) << config->block_bits;

  return (addr & tag_mask & block_mask) >> config->block_bits;
}

uint64_t extract_tag(cache_config_t *config, uint64_t addr) {
  uint64_t tag_mask = build_tag_mask(config);
  uint64_t tag = (addr & tag_mask) >> (config->set_idx_bits + config->block_bits);

  return tag;
}

uint64_t calculate_num_lines(cache_config_t *config) {
  return (0x1l << config->set_idx_bits) * config->lines_per_set;
}

uint64_t calculate_total_line_space(cache_config_t *config) {
  return calculate_num_lines(config) * sizeof(cache_line_t);
}

cache_t initialize_cache(cache_config_t *config, cache_line_t *line_array) {
  uint64_t num_lines = calculate_num_lines(config);
  cache_line_t *current_line = line_array;

  for (uint64_t i = 0; i < num_lines; i++) {
    current_line->tag = 0;
    current_line->valid = 0;
    current_line++;
  }

  return line_array;
}

cache_result_t access_cache(cache_config_t *config, cache_t cache, uint64_t addr) {
  uint64_t set_idx = extract_set_idx(config, addr);
  uint64_t tag = extract_tag(config, addr);
  cache_line_t *line_lb = cache + (set_idx * config->lines_per_set);
  cache_line_t *line_ub = cache + ((set_idx + 1) * config->lines_per_set);

  int valid_line_count = 0;
  cache_line_t *first_invalid_line = NULL;
  cache_result_t result = MISS;

  for (cache_line_t *line = line_lb; line < line_ub; line++) {
    valid_line_count += line->valid;
    if (tag == line->tag && line->valid) {
      result = HIT;
    } else if (!line->valid && first_invalid_line == NULL) {
      first_invalid_line = line;
    }
  }

  if (result == MISS && valid_line_count == config->lines_per_set) {
    result = EVICTION;
  }

  if (result == MISS || result == EVICTION) {
    if (first_invalid_line != NULL) {
      first_invalid_line->valid = 1;
      first_invalid_line->tag = tag;
    }
  }

  return result;
}
