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

  free(trace_file);
  free(config);
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

  free(trace_file);
  free(config);
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

  free(trace_file);
  free(config);
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

  free(trace_file);
  free(config);
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

  free(trace_file);
  free(config);
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

  free(trace_file);
  free(config);
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

  free(config);
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

  free(config);
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

  free(config);
  return NULL;
}

char *test_accessing_cache(void) {
  cache_config_t *config = malloc(sizeof(cache_config_t));
  config->set_idx_bits = 4;
  config->lines_per_set = 4;
  config->block_bits = 4;

  cache_line_t *lines = malloc(calculate_total_line_space(config));
  cache_t cache = initialize_cache(config, lines);

  // All in the same set, but each with a different tag
  uint64_t first_addr  = 0x0000000000000050;
  uint64_t second_addr = 0x0000000000000150;
  uint64_t third_addr  = 0x0000000000000250;
  uint64_t fourth_addr = 0x0000000000000350;
  uint64_t fifth_addr  = 0x0000000000000450;

  cache_result_t first_miss = access_cache(config, cache, first_addr);
  cache_result_t first_hit = access_cache(config, cache, first_addr);
  cache_result_t second_miss = access_cache(config, cache, second_addr);
  cache_result_t second_hit = access_cache(config, cache, second_addr);
  cache_result_t third_miss = access_cache(config, cache, third_addr);
  cache_result_t third_hit = access_cache(config, cache, third_addr);
  cache_result_t fourth_miss = access_cache(config, cache, fourth_addr);
  cache_result_t fourth_hit = access_cache(config, cache, fourth_addr);
  cache_result_t fifth_eviction = access_cache(config, cache, fifth_addr);

  mu_assert(first_miss == MISS, "Did not miss first addr");
  mu_assert(first_hit == HIT, "Did not hit first addr");
  mu_assert(second_miss == MISS, "Did not miss second addr");
  mu_assert(second_hit == HIT, "Did not hit second addr");
  mu_assert(third_miss == MISS, "Did not miss third addr");
  mu_assert(third_hit == HIT, "Did not hit third addr");
  mu_assert(fourth_miss == MISS, "Did not miss fourth addr");
  mu_assert(fourth_hit == HIT, "Did not hit fourth addr");
  mu_assert(fifth_eviction == EVICTION, "Did not evict fifth addr");

  free(config);
  free(lines);

  return NULL;
}

char *test_eviction_policy(void) {
  cache_config_t *config = malloc(sizeof(cache_config_t));
  config->set_idx_bits = 4;
  config->lines_per_set = 4;
  config->block_bits = 4;

  cache_line_t *lines = malloc(calculate_total_line_space(config));
  cache_t cache = initialize_cache(config, lines);

  // All in the same set, but each with a different tag
  uint64_t first_addr  = 0x0000000000000050;
  uint64_t second_addr = 0x0000000000000150;
  uint64_t third_addr  = 0x0000000000000250;
  uint64_t fourth_addr = 0x0000000000000350;
  uint64_t fifth_addr  = 0x0000000000000450;

  cache_result_t first_miss = access_cache(config, cache, first_addr);
  cache_result_t second_miss = access_cache(config, cache, second_addr);
  cache_result_t third_miss = access_cache(config, cache, third_addr);
  cache_result_t fourth_miss = access_cache(config, cache, fourth_addr);
  cache_result_t first_hit = access_cache(config, cache, first_addr);
  cache_result_t second_hit = access_cache(config, cache, second_addr);
  cache_result_t fifth_miss = access_cache(config, cache, fifth_addr);

  cache_result_t first_hit_2 = access_cache(config, cache, first_addr);
  cache_result_t second_hit_2 = access_cache(config, cache, second_addr);
  cache_result_t fourth_hit_2 = access_cache(config, cache, fourth_addr);
  cache_result_t fifth_hit_2 = access_cache(config, cache, fifth_addr);

  cache_result_t third_miss_2 = access_cache(config, cache, third_addr);

  mu_assert(first_miss == MISS, "Did not miss first addr");
  mu_assert(second_miss == MISS, "Did not miss second addr");
  mu_assert(third_miss == MISS, "Did not miss third addr");
  mu_assert(fourth_miss == MISS, "Did not miss fourth addr");
  mu_assert(first_hit == HIT, "Did not hit first addr after insert");
  mu_assert(second_hit == HIT, "Did not hit second addr after insert");
  mu_assert(fifth_miss == EVICTION, "Did not evict after fifth addr");

  mu_assert(first_hit_2 == HIT, "Did not hit first addr after eviction");
  mu_assert(second_hit_2 == HIT, "Did not hit second addr after eviction");
  mu_assert(fourth_hit_2 == HIT, "Did not hit fourth addr after eviction");
  mu_assert(fifth_hit_2 == HIT, "Did not hit fifth addr after eviction");

  mu_assert(third_miss_2 == EVICTION, "Did not miss third addr after eviction");

  free(config);
  free(lines);
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
  mu_run_test(test_extracts_tag_bits);
  mu_run_test(test_calculate_line_space);
  mu_run_test(test_accessing_cache);
  mu_run_test(test_eviction_policy);

  return NULL;
}

RUN_TESTS(all_tests);
