#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define LENGTH_BUFFER 1000

int main(int argc, char *argv[]) {
  char BUFFER[LENGTH_BUFFER + 1] = {0};
  FILE *fptr = NULL;
  FILE *w0fptr = NULL;
  FILE *w1fptr = NULL;
  int status = 0;

  if (argc != 2) {
    printf("Wrong count arguments\n");
    exit(1);
  }

  fptr = fopen(argv[1], "r");

  if (fptr == NULL) {
    printf("Failed open file %s\n", argv[1]);
    exit(1);
  }

  pid_t p = fork(); // форкаемся

  if (p < 0) {
    printf("Failed to fork\n");
    exit(1);
  } else if (p == 0) { // форк или не форк
    w0fptr = fopen("fork_text.txt", "rw");
  } else {
    w1fptr = fopen("not_fork_text.txt", "rw");
  }

  if (p == 0) {
    while (!feof(fptr)) {
      char c = fgetc(fptr);
      fputc(c, w0fptr);
    }
  } else {
    while (!feof(fptr)) {
      char c = fgetc(fptr);
      fputc(c, w1fptr);
    }
  }

  if (p == 0) {
    BUFFER[LENGTH_BUFFER] = '\0';
    while (!feof(w0fptr)) {
      fread(BUFFER, sizeof(char), LENGTH_BUFFER, w0fptr);
      printf("%s", BUFFER);
    }
    fclose(w0fptr);
    fclose(fptr);
  } else {
    wait(&status);
    BUFFER[LENGTH_BUFFER] = '\0';
    while (!feof(w1fptr)) {
      fread(BUFFER, sizeof(char), LENGTH_BUFFER, w1fptr);
      printf("%s", BUFFER);
    }
    fclose(w1fptr);
    fclose(fptr);
  }

  // Как я понял, у форка нет доступа к адресу указателя на чтения файла поэтому
  // полчается как то не очень
  //
  // Можно использовать мутекс чтобы сначало читалось поочередно

  return EXIT_SUCCESS;
}
