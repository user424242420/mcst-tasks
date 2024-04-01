#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

size_t i, j, k = {0};

void handle_sigint(int sig) {
  // printf("Caught signal %d\n", sig);
  printf("I=%zu J=%zu\n", i, j);
  signal(SIGINT, SIG_DFL);
}

int main(int argc, char *argv[]) {
  int a[1000][1000];
  int b[1000][1000];
  int **c; // еще один двумерный массив с 1000 * 1000 программа не потянула и
           // пришлось его коллокейтить

  c = calloc(sizeof(int *), 1000);
  for (i = 0; i < 1000; i++) {
    c[i] = calloc(sizeof(int), 1000);
  }

  for (i = 0; i < 1000; i++) {
    for (j = 0; j < 1000; j++) {
      a[i][j] = (int)rand() % 1000;
      b[i][j] = (int)rand() % 1000;
      c[i][j] = (int)rand() % 1000;
    }
  }
  signal(SIGINT, handle_sigint);
  int s = 0;
  for (i = 0; i < 1000; i++) {
    for (j = 0; j < 1000; j++) {
      s = 0;
      for (k = 0; k < 1000; k++) {
        s += a[i][k] * b[k][j];
      }
      sleep(1);
      c[i][j] = 0;
    }
  }

  return EXIT_SUCCESS;
}
