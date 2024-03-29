#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char small_opt[] = {'m', 'c', 's', 't', '\0'};

const char *big_opt = "--elbrus=\0";
const char big_opt_arr[6][4] = {"1c+\0", "2c+\0", "2c3\0",
                                "4c\0",  "8c\0",  "16c\0"};

int main(int argc, char *argv[]) {
  char *s_opts = calloc(argc, sizeof(char));
  unsigned int count_s_opts = 0;

  int *s_big_opts = calloc(argc, sizeof(char));
  unsigned int count_s_big_opts = 0;

  char **non_opts = calloc(argc, sizeof(char **));
  unsigned int count_s_non_opts = 0;

  bool valid = false;

  for (unsigned int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == '\0') {
        printf("Incorrect input params\n");
        exit(1);
      }
      if (argv[i][1] == '-') {
        if (strncmp(argv[i], big_opt, strlen(big_opt)) == 0 &&
            strlen(argv[i]) == 12) {

          valid = false;

          for (unsigned int l = 0; l < 6; l++) {
            if (strcmp(big_opt_arr[l], argv[i] + 9) == 0) {

              valid = true;
              s_big_opts[count_s_big_opts] = l;
              count_s_big_opts++;
              break;
            }
          }
          if (valid == false) {
            printf("Invalid big opts: %s", argv[i]);
            exit(1);
          }

        } else {
          printf("Error big param\n");
          exit(1);
        }

      } else {
        for (unsigned int j = 1; j < strlen(argv[i]); j++) {
          valid = false;
          for (unsigned l = 0; l < strlen(small_opt); l++) {
            if (argv[i][j] == small_opt[l]) {
              valid = true;
              s_opts[count_s_opts] = argv[i][j];
              count_s_opts++;
              break;
            }
          }
          if (valid == false) {
            printf("Invalid opts: -%c", argv[i][j]);
            exit(1);
          }
        }
      }
    } else {
      non_opts[count_s_non_opts] = argv[i];
      count_s_non_opts++;
    }
  }
  printf("Small opts: ");
  for (unsigned int i = 0; i < count_s_opts; i++) {
    printf("-%c ", s_opts[i]);
  }

  printf("\nBig opt: ");
  for (unsigned int i = 0; i < count_s_big_opts; i++) {
    printf("--elbrus=%s ", big_opt_arr[s_big_opts[i]]);
  }
  printf("\nNon opt: ");
  for (unsigned int i = 0; i < count_s_non_opts; i++) {
    printf("%s ", non_opts[i]);
  }

  printf("\n");

  free(s_big_opts);
  free(s_opts);
  free(non_opts);

  return EXIT_SUCCESS;
}
