#include "libcsim.h"

int TRACE_FILE_NAME_LENGTH = 32;
void promote_line(cache_line_t *first_line, cache_line_t *line, uint64_t num_lines);

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

uint64_t calculate_num_sets(cache_config_t *config) {
  return (0x1l << config->set_idx_bits);
}

uint64_t calculate_num_lines(cache_config_t *config) {
  return calculate_num_sets(config) * config->lines_per_set;
}

uint64_t calculate_total_line_space(cache_config_t *config) {
  return calculate_num_lines(config) * sizeof(cache_line_t);
}

cache_t initialize_cache(cache_config_t *config, cache_line_t *line_array) {
  uint64_t num_lines = calculate_num_lines(config);
  uint64_t num_sets = calculate_num_sets(config);
  cache_line_t *current_line = line_array;

  for (uint64_t i = 0; i < num_lines; i++) {
    current_line->tag = -1;
    current_line->valid = 0;
    current_line->prev = NULL;
    current_line->next = NULL;
    current_line++;
  }

  for (uint64_t set_idx = 0; set_idx < num_sets; set_idx++) {
    cache_line_t *first_line = line_array + (set_idx * config->lines_per_set);
    cache_line_t *last_line = line_array + ((set_idx + 1) * config->lines_per_set) - 1;
    for (cache_line_t *line = first_line; line < last_line; line++) {
      line->next = line + 1;
      (line + 1)->prev = line;
    }
  }

  return line_array;
}

cache_result_t access_cache(cache_config_t *config, cache_t cache, uint64_t addr) {
  uint64_t set_idx = extract_set_idx(config, addr);
  uint64_t tag = extract_tag(config, addr);

  cache_line_t *first_line = cache + (set_idx * config->lines_per_set);
  cache_line_t *most_recent_line;
  for (int i = 0; i < config->lines_per_set; i++) {
    if ((first_line + i)->prev == NULL) {
      most_recent_line = first_line + i;
    }
  }

  cache_line_t *line = most_recent_line;
  cache_line_t *last_line;
  cache_result_t result = EVICTION;

  while(result == EVICTION && line != NULL) {
    if (!line->valid) {
      result = MISS;
      line->valid = 1;
      line->tag = tag;
      promote_line(most_recent_line, line, config->lines_per_set);
    } else if (line->valid && tag == line->tag) {
      result = HIT;
      promote_line(most_recent_line, line, config->lines_per_set);
    } else {
      last_line = line;
      line = line->next;
    }
  }

  if (result == EVICTION) {
    last_line->tag = tag;
    last_line->valid = 1;
    promote_line(most_recent_line, last_line, config->lines_per_set);
  }

  return result;
}

void promote_line(cache_line_t *most_recent_line, cache_line_t *line, uint64_t num_lines) {
  if (line == most_recent_line) {
    return;
  }
  if (line->next != NULL) {
    line->next->prev = line->prev;
  }
  line->prev->next = line->next;
  line->next = most_recent_line;
  line->prev = NULL;
  most_recent_line->prev = line;
}

void parse_line(char *line_str, line_t* line) {
  uint64_t addr = 0;
  char cmd = '\0';

  if (isspace(*line_str)) {
    sscanf(line_str, " %c %lx", &cmd, &addr);
  } else {
    sscanf(line_str, "%c %lx", &cmd, &addr);
  }

  line->addr = addr;
  if (cmd == 'I') {
    line->command = INSTR;
  } else if (cmd == 'L') {
    line->command = LOAD;
  } else if (cmd == 'M') {
    line->command = MODIFY;
  } else if (cmd == 'S') {
    line->command = STORE;
  }
}

cache_result_t simulate_cache_access(cache_config_t *config, cache_t cache, line_t *line, counter_t *counter) {
  cache_result_t result;
  cache_result_t modify_result; // counted in sub-calls

  switch (line->command) {
    case INSTR:
      break;
    case LOAD:
    case STORE:
      result = access_cache(config, cache, line->addr);
      break;
    case MODIFY:
      line->command = LOAD;
      modify_result = simulate_cache_access(config, cache, line, counter);
      line->command = STORE;
      // modify will always end with a hit
      simulate_cache_access(config, cache, line, counter);
      line->command = MODIFY;
      break;
  }

  switch (result) {
    case HIT:
      counter->hits += 1;
      break;
    case MISS:
      counter->misses += 1;
      break;
    case EVICTION:
      counter->evictions += 1;
      break;
    default:
      break;
  }

  if (line->command == MODIFY) {
    return modify_result;
  } else {
    return result;
  }
}
