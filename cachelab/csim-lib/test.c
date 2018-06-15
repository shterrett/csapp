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
  int set_idx_bits = 0;
  int lines_per_set = 0;
  int block_bits = 0;
  char* trace_file = malloc(sizeof(char) * 32);

  int help = init(argc, argv, &verbose, &set_idx_bits, &lines_per_set, &block_bits, trace_file);

  mu_assert(help == 1, "Did not set help bit");

  return NULL;
}

char *test_verbose(void) {
  int argc = 2;
  char *argv[] = { "bin/csim", "-v" };

  int verbose = 0;
  int set_idx_bits = 0;
  int lines_per_set = 0;
  int block_bits = 0;
  char* trace_file = malloc(sizeof(char) * 32);

  init(argc, argv, &verbose, &set_idx_bits, &lines_per_set, &block_bits, trace_file);

  mu_assert(verbose == 1, "Did not set verbose bit");

  return NULL;
}

char *test_no_help_or_verbose(void) {
  int argc = 1;
  char *argv[] = { "bin/csim" };

  int verbose = 0;
  int set_idx_bits = 0;
  int lines_per_set = 0;
  int block_bits = 0;
  char* trace_file = malloc(sizeof(char) * 32);

  int help = init(argc, argv, &verbose, &set_idx_bits, &lines_per_set, &block_bits, trace_file);

  mu_assert(help == 0, "Inappropriately set help bit");
  mu_assert(verbose == 0, "Inappropriately set verbose bit");

  return NULL;
}

char *test_initializes_variables(void) {
  int argc = 9;
  char *argv[] = {"bin/csim", "-s", "5", "-E", "7", "-b", "9", "-t", "tracefile.txt"};

  int verbose = 0;
  int set_idx_bits = 0;
  int lines_per_set = 0;
  int block_bits = 0;
  char* trace_file = malloc(sizeof(char) * 32);

  int required = init(argc, argv, &verbose, &set_idx_bits, &lines_per_set, &block_bits, trace_file);

  mu_assert(set_idx_bits == 5, "Failed to set set_idx_bits");
  mu_assert(lines_per_set == 7, "Failed to set lines_per_set");
  mu_assert(block_bits == 9, "Failed to set block_bits");
  mu_assert(strcmp(trace_file, "tracefile.txt") == 0, "Failed to set trace_file");
  mu_assert(required == 8, "did not count args properly");

  return NULL;
}

char *test_not_enough_variables(void) {
  int argc = 7;
  char *argv[] = {"bin/csim", "-E", "7", "-b", "9", "-t", "tracefile.txt"};

  int verbose = 0;
  int set_idx_bits = 0;
  int lines_per_set = 0;
  int block_bits = 0;
  char* trace_file = malloc(sizeof(char) * 32);

  int required = init(argc, argv, &verbose, &set_idx_bits, &lines_per_set, &block_bits, trace_file);

  mu_assert(required == 6, "did not count args properly");

  return NULL;
}

char *test_help_makes_arg_count_odd(void) {
  int argc = 10;
  char *argv[] = {"bin/csim", "-s", "5", "-E", "7", "-b", "9", "-h", "-t", "tracefile.txt"};

  int verbose = 0;
  int set_idx_bits = 0;
  int lines_per_set = 0;
  int block_bits = 0;
  char* trace_file = malloc(sizeof(char) * 32);

  int required = init(argc, argv, &verbose, &set_idx_bits, &lines_per_set, &block_bits, trace_file);

  mu_assert(required == 9, "did not count args properly");

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

  return NULL;
}

RUN_TESTS(all_tests);
