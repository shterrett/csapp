#include "minunit.h"
#include <assert.h>
#include "libcsim.h"

char *test_compilation(void) {
  mu_assert(1 == 1, "It failed?");
  return NULL;
}

char *test_help(void) {
  int argc = 2;
  char *argv[] = { "bin/csim", "-h" };

  int verbose = 0;
  cache_config_t *config = malloc(sizeof(cache_config_t));
  char* trace_file = malloc(sizeof(char) * 32);

  int help = init(argc, argv, &verbose, config, trace_file);

  mu_assert(help == 1, "Did not set help bit");

  return NULL;
}

char *test_verbose(void) {
  int argc = 2;
  char *argv[] = { "bin/csim", "-v" };

  int verbose = 0;
  cache_config_t *config = malloc(sizeof(cache_config_t));
  char* trace_file = malloc(sizeof(char) * 32);

  init(argc, argv, &verbose, config, trace_file);

  mu_assert(verbose == 1, "Did not set verbose bit");

  return NULL;
}

char *test_no_help_or_verbose(void) {
  int argc = 1;
  char *argv[] = { "bin/csim" };

  int verbose = 0;
  cache_config_t *config = malloc(sizeof(cache_config_t));
  char* trace_file = malloc(sizeof(char) * 32);

  int help = init(argc, argv, &verbose, config, trace_file);

  mu_assert(help == 0, "Inappropriately set help bit");
  mu_assert(verbose == 0, "Inappropriately set verbose bit");

  return NULL;
}

char *test_initializes_variables(void) {
  int argc = 9;
  char *argv[] = {"bin/csim", "-s", "5", "-E", "7", "-b", "9", "-t", "tracefile.txt"};

  int verbose = 0;
  cache_config_t *config = malloc(sizeof(cache_config_t));
  char* trace_file = malloc(sizeof(char) * 32);

  int required = init(argc, argv, &verbose, config, trace_file);

  mu_assert(config->set_idx_bits == 5, "Failed to set set_idx_bits");
  mu_assert(config->lines_per_set == 7, "Failed to set lines_per_set");
  mu_assert(config->block_bits == 9, "Failed to set block_bits");
  mu_assert(strcmp(trace_file, "tracefile.txt") == 0, "Failed to set trace_file");
  mu_assert(required == 8, "did not count args properly");

  return NULL;
}

char *test_not_enough_variables(void) {
  int argc = 7;
  char *argv[] = {"bin/csim", "-E", "7", "-b", "9", "-t", "tracefile.txt"};

  int verbose = 0;
  cache_config_t *config = malloc(sizeof(cache_config_t));
  char* trace_file = malloc(sizeof(char) * 32);

  int required = init(argc, argv, &verbose, config, trace_file);

  mu_assert(required == 6, "did not count args properly");

  return NULL;
}

char *test_help_makes_arg_count_odd(void) {
  int argc = 10;
  char *argv[] = {"bin/csim", "-s", "5", "-E", "7", "-b", "9", "-h", "-t", "tracefile.txt"};

  int verbose = 0;
  cache_config_t *config = malloc(sizeof(cache_config_t));
  char* trace_file = malloc(sizeof(char) * 32);

  int required = init(argc, argv, &verbose, config, trace_file);

  mu_assert(required == 9, "did not count args properly");

  return NULL;
}

char *test_extracts_set_bits_from_address(void) {
  uint64_t addr = 0x0000000000180000;
  cache_config_t *config = malloc(sizeof(cache_config_t));
  config->set_idx_bits = 32;
  config->lines_per_set = 1;
  config->block_bits = 16;

  uint64_t set_idx = extract_set_idx(config, addr);

  mu_assert(set_idx == 24, "extracted wrong set bits");

  return NULL;
}

char *test_extracts_block_offset(void) {
  uint64_t addr = 0x1111111111110008;
  cache_config_t *config = malloc(sizeof(cache_config_t));
  config->set_idx_bits = 32;
  config->lines_per_set = 1;
  config->block_bits = 16;

  uint64_t block_offset = extract_block_offset(config, addr);

  mu_assert(block_offset == 8, "extracted wrong block offset");

  return NULL;
}

char *test_extracts_tag_bits(void) {
  uint64_t addr = 0x0008111111111111;
  cache_config_t *config = malloc(sizeof(cache_config_t));
  config->set_idx_bits = 32;
  config->lines_per_set = 1;
  config->block_bits = 16;

  uint64_t tag = extract_tag(config, addr);

  mu_assert(tag == 8, "extracted wrong tag");

  return NULL;
}

char *test_calculate_line_space(void) {
  cache_config_t *config = malloc(sizeof(cache_config_t));
  config->set_idx_bits = 32;
  config->lines_per_set = 4;
  config->block_bits = 16;

  uint64_t line_space = calculate_total_line_space(config);

  mu_assert(line_space == 17179869184 * sizeof(cache_line_t),
            "Did not correctly calculate line_space: 2^s * E * struct size");

  return NULL;
}

char *test_calculate_block_space(void) {
  cache_config_t *config = malloc(sizeof(cache_config_t));
  config->set_idx_bits = 32;
  config->lines_per_set = 4;
  config->block_bits = 16;

  uint64_t block_space = calculate_total_block_space(config);

  mu_assert(block_space == 17179869184 * 65536,
            "Did not correctly calculate the block space: num_lines * 2^b");

  return NULL;
}

char *all_tests(void) {
  mu_suite_start();
  mu_run_test(test_compilation);
  mu_run_test(test_help);
  mu_run_test(test_verbose);
  mu_run_test(test_no_help_or_verbose);
  mu_run_test(test_initializes_variables);
  mu_run_test(test_not_enough_variables);
  mu_run_test(test_help_makes_arg_count_odd);
  mu_run_test(test_extracts_set_bits_from_address);
  mu_run_test(test_extracts_block_offset);
  mu_run_test(test_extracts_tag_bits);
  mu_run_test(test_calculate_line_space);
  mu_run_test(test_calculate_block_space);

  return NULL;
}

RUN_TESTS(all_tests);
