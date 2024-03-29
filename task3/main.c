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

#define swap(x, y)                                                             \
  do {                                                                         \
    unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1];  \
    memcpy(swap_temp, &y, sizeof(x));                                          \
    memcpy(&y, &x, sizeof(x));                                                 \
    memcpy(&x, swap_temp, sizeof(x));                                          \
  } while (0)

unsigned int *bubble_sort(char **a_lines, unsigned int lines,
                          unsigned int file_len);
unsigned int *shaker_sort(char **a_lines, unsigned int lines,
                          unsigned int file_len);

unsigned int *insertion_sort(char **a_lines, unsigned int lines,
                             unsigned int file_len);

int main(int argc, char *argv[]) {
  FILE *fptr = NULL;
  unsigned int file_size = 0;
  char *buffer = NULL;
  char **a_lines = NULL;
  unsigned int lines = 0;
  unsigned int *len_lines = NULL;
  unsigned int *addresses = NULL;

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

  if (file_size == 0) {
    printf("File dont have content, exit\n");
    exit(1);
  }

  buffer = calloc(file_size, 1);

  char ch;
  while (!feof(fptr)) {
    ch = fgetc(fptr);
    if (ch == '\n') {
      lines++;
    }
  }

  len_lines = calloc(lines, sizeof(int));
  a_lines = calloc(lines, sizeof(char *));
  fseek(fptr, 0, SEEK_SET);

  size_t curr_len = 0;
  unsigned int i = 0;
  char *line = NULL;
  while (!feof(fptr)) {
    ch = fgetc(fptr);
    buffer[curr_len] = ch;
    curr_len++;
    if (ch == '\n') {
      buffer[curr_len] = '\0';
      curr_len++;
      line = calloc(curr_len, sizeof(char));
      strncpy(line, (const char *)buffer, curr_len);
      a_lines[i] = line;
      len_lines[i] = curr_len;
      i++;
      curr_len = 0;
    }
  }
  free(buffer);

  fclose(fptr);
  switch (*argv[3]) {
  case '1':
    addresses = bubble_sort(a_lines, lines, file_size);
    break;
  case '2':
    addresses = shaker_sort(a_lines, lines, file_size);
    break;
  case '3':
    addresses = insertion_sort(a_lines, lines, file_size);
    break;

  default:
    printf("Not valid type of sort\n");
    exit(1);
  }

  fptr = fopen(argv[2], "w");

  if (fptr == NULL) {
    printf("Problems to write with file with name %s, exit\n", argv[2]);
    exit(1);
  }

  for (int i = 0; i < lines; i++) {
    if (a_lines[addresses[i]][0] != '\n' && a_lines[addresses[i]][0] != '\0') {
      fputs(a_lines[addresses[i]], fptr);
      // fprintf(fptr, "%s", a_lines[addresses[i]]);
      printf("%s", a_lines[addresses[i]]);
    }
    free(a_lines[addresses[i]]);
  }

  fclose(fptr);

  free(addresses);
  free(len_lines);
  free(a_lines);

  return EXIT_SUCCESS;
}

unsigned int *bubble_sort(char **a_lines, unsigned int lines,
                          unsigned int file_len) {
  unsigned int *addresses = calloc(lines, sizeof(unsigned int));
  bool f = 0;
  int temp = 0;

  for (unsigned int i = 0; i < lines; i++) {
    addresses[i] = i;
  }

  for (unsigned int j = 1; j < lines; j++) {
    f = 0;
    for (unsigned int i = 0; i < lines - j; i++) {
      if (a_lines[addresses[i]][0] == '\n') {
        swap(addresses[i], addresses[i + 1]);
        f = 1;
      } else if (a_lines[addresses[i + 1]][0] == '\n') {
      } else if (strcmp(a_lines[addresses[i]], a_lines[addresses[i + 1]]) > 0) {
        swap(addresses[i], addresses[i + 1]);
        f = 1;
      };
    }
    if (f == 0) {
      break;
    }
  }
  return addresses;
}
unsigned int *shaker_sort(char **a_lines, unsigned int lines,
                          unsigned int file_len) {
  unsigned int *addresses = NULL;
  bool f;
  int temp;

  addresses = calloc(lines, sizeof(int));
  for (unsigned int i = 0; i < lines; i++) {
    addresses[i] = i;
  }

  int left = 0;
  int right = lines - 1;
  while (left <= right) {
    for (int i = right; i > left; --i) {
      if (strcmp(a_lines[addresses[i - 1]], a_lines[addresses[i]]) > 0) {
        swap(addresses[i - 1], addresses[i]);
      }
    }
    ++left;
    for (int i = left; i < right; ++i) {
      if (strcmp(a_lines[addresses[i]], a_lines[addresses[i + 1]]) > 0) {
        swap(addresses[i], addresses[i + 1]);
      }
    }
    --right;
  }
  return addresses;
}

unsigned int *insertion_sort(char **a_lines, unsigned int lines,
                             unsigned int file_len) {
  unsigned int *addresses = NULL;
  bool f;
  int temp;

  addresses = calloc(lines, sizeof(int));
  for (unsigned int i = 0; i < lines; i++) {
    addresses[i] = i;
  }

  for (int i = 1; i < lines; i++) {
    int j = i - 1;
    while (j >= 0 &&
           strcmp(a_lines[addresses[j]], a_lines[addresses[j + 1]]) > 0) {
      swap(addresses[j], addresses[j + 1]);
      j--;
    }
  }
  return addresses;
}
