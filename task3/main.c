#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })
unsigned int *bubble_sort(char **a_lines, unsigned int lines,
                          unsigned int file_len);

int main(int argc, char *argv[]) {
  FILE *fptr;
  size_t file_size;
  char *buffer;
  char **a_lines;
  unsigned int lines;
  unsigned int *len_lines;
  unsigned int *addresses;

  if (argc != 4) {
    printf("Not valid count argumets\n");
    exit(1);
  }

  fptr = fopen(argv[1], "r");

  if (fptr == NULL) {
    printf("File with name %s, doesnt exist, exit\n", argv[1]);
    exit(1);
  }

  fseek(fptr, 0, SEEK_END);
  file_size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  buffer = malloc(file_size);

  char ch;
  while (!feof(fptr)) {
    ch = fgetc(fptr);
    if (ch == '\n') {
      lines++;
    }
  }

  len_lines = malloc(lines * sizeof(int));
  a_lines = malloc(lines * sizeof(char *));
  fseek(fptr, 0, SEEK_SET);

  size_t curr_len = 0;
  unsigned int i = 0;
  char *line;
  while (!feof(fptr)) {
    ch = fgetc(fptr);
    buffer[curr_len] = ch;
    curr_len++;
    if (ch == '\n') {
      line = malloc(curr_len * sizeof(char));
      strncpy(line, (const char *)buffer, curr_len);
      a_lines[i] = line;
      len_lines[i] = curr_len;
      i++;
      curr_len = 0;
    }
  }

  addresses = bubble_sort(a_lines, lines, file_size);

  for (int i = 0; i < lines; i++) {
    printf("%s", a_lines[addresses[i]]);
  }

  return EXIT_SUCCESS;
}

unsigned int *bubble_sort(char **a_lines, unsigned int lines,
                          unsigned int file_len) {
  unsigned int *addresses;
  bool f;
  int temp;

  addresses = malloc(lines * sizeof(int));
  for (unsigned int i = 0; i < lines; i++) {
    addresses[i] = i;
  }

  for (unsigned int j = 1; j < lines; j++) {
    f = 0;
    for (unsigned int i = 0; i < lines - j; i++) {

      if (strncmp(a_lines[addresses[i]], a_lines[addresses[i + 1]],
                  min(strlen(a_lines[addresses[i]]),
                      strlen(a_lines[addresses[i + 1]]))) > 0) {
        temp = addresses[i];
        addresses[i] = addresses[i + 1];
        addresses[i + 1] = temp;
        f = 1;
      };
    }
    if (f == 0) {
      break;
    }
  }
  return addresses;
}
