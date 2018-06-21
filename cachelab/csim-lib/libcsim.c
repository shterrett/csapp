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
