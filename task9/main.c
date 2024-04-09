
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

#define RAND_RANGE 1000

int main(int argc, char *argv[]) {
  srand(time(NULL));
  bool t = 0;
  size_t c = 0;
  int *a = NULL;
  if (argc == 2) {
    t = ((argv[1][0] == 'r') || (argv[1][1] == 'r'));
  }
  printf("Введите количество элементов:\n");
  scanf("%zu", &c);
  a = calloc(sizeof(int), c);

  for (size_t i = 0; i < c; i++) {
    if (!t) {
      scanf("%i", &a[i]);
    } else {
      a[i] = (rand() % (RAND_RANGE * 2) - RAND_RANGE);
    }
  }

  int max_num = 0;
  for (size_t i = 0; i < c; i++) {
    max_num = max(max_num, a[i]);
  }

  if (max_num < 0) {
    printf("Max sum: %i\n", max_num);
    exit(0);
  }

  int max_so_far = 0;

  int max_ending_here = 0;

  for (size_t i = 0; i < c; i++) {
    max_ending_here = max_ending_here + a[i];
    max_ending_here = max(max_ending_here, 0);
    max_so_far = max(max_so_far, max_ending_here);
  }

  printf("Max sum: %i\n", max_so_far);

  return EXIT_SUCCESS;
}
