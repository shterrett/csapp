#include "minunit.h"
#include <assert.h>

char *test_compilation(void) {
  mu_assert(1 == 1, "It failed?");
  return NULL;
}

char *all_tests(void) {
  mu_suite_start();
  mu_run_test(test_compilation);

  return NULL;
}

RUN_TESTS(all_tests);
