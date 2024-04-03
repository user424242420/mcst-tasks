#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

#define BUFFER_LENGTH 1000

const char ALLOWED_CHARS[] = {" ABCDEFGHIJKLMNOPQRSTUVWXYZ()\n\0"};

struct st {
  char first_real;
  struct st *first;
  char operator[10];
  struct st *second;
  char second_real;

  bool result;
};

char NULL_STRING[3] = {0};

int alphabet[50] = {0};

struct st *parse(char *buffer, size_t start, size_t end) {

  int count = 0;
  struct st *obj;
  obj = calloc(sizeof(struct st), 1);
  for (size_t i = start; i < end; i++) {
    if (buffer[i] == '(') {
      for (size_t j = end - 1; j > 0; j--) {

        if (buffer[j] == ')') {
          int i_1 = i;
          if (buffer[i + 1] == '(') {
            i_1 += 2;
            int count = -1;
            for (; count != 0 && i_1 < end; i_1++) {
              if (buffer[i_1] == '(') {
                count--;
              } else if (buffer[i_1] == ')') {
                count++;
              }
            }
            printf("i i_1 %zu %i\n", i, i_1);
            obj->first = parse(buffer, i + 1, i_1);
            i_1 += 1;
          } else {
            obj->first_real = buffer[i_1 + 1];

            alphabet[(int)toupper(obj->first_real) - 'A'] = 1;
            i_1 += 3;
          }

          int j_1 = j;
          if (buffer[j - 1] == ')') {
            j_1 -= 2;
            int count = -1;
            for (; count != 0 && j_1 > start; j_1--) {

              if (buffer[j_1] == ')') {
                count--;
              } else if (buffer[j_1] == '(') {
                count++;
              }
            }

            obj->second = parse(buffer, j_1 + 1, j);

          } else {

            obj->second_real = buffer[j_1 - 1];

            alphabet[(int)(toupper(obj->second_real) - 'A')] = 1;
            j_1 -= 2;
          }

          strncpy(obj->operator, buffer + i_1, j_1 - i_1);
        }
        if ((obj->first != NULL || obj->first_real != NULL_STRING[0]) &&
            (obj->second != NULL || obj->second_real != NULL_STRING[0])) {
          return obj;
        }
        if (i >= j) {

          printf("Invalid parse %zu %zu\n", i, j);
          break;
        }
      }
    }
  }

  return obj;
}

bool calc_result(struct st *obj, bool usefree) {
  bool first, second;
  if (obj->first != NULL) {
    first = calc_result(obj->first, usefree);
  } else {
    first = alphabet[toupper(obj->first_real) - 'A'];
  }
  if (obj->second != NULL) {
    second = calc_result(obj->second, usefree);
  } else {
    second = alphabet[toupper(obj->second_real) - 'A'];
  }

  if (strncmp(obj->operator, "AND", 3) == 0) {
    obj->result = first && second;
  }

  else if (strncmp(obj->operator, "OR", 2) == 0) {
    obj->result = first || second;
  }

  else if (strncmp(obj->operator, "NOT AND", 7) == 0) {
    obj->result = !(first && second);
  }

  else if (strncmp(obj->operator, "NOT OR", 6) == 0) {
    obj->result = !(first || second);
  } else {
    printf("Unimplemented operation or wrong string\n");
    exit(1);
  }

  if (usefree) {
    if (obj->first != NULL) {
      free(obj->first);
    }
    if (obj->second != NULL) {
      free(obj->second);
    }
  }

  return obj->result;
}

int main(int argc, char *argv[]) {
  struct st *obj = {0};

  size_t count_s = 0;
  size_t len_str = 0;
  char BUFFER[BUFFER_LENGTH + 1] = {0};

  fgets(BUFFER, BUFFER_LENGTH, stdin);

  len_str = strlen(BUFFER);

  // for (; BUFFER[len_str] != ' '; len_str--)
  //;
  // len_str -= 2;

  printf("CHAR LAST %zu %c\n", len_str, BUFFER[len_str]);

  int count = 0;

  for (size_t i = 0; i < BUFFER_LENGTH; i++) { // basic check
    bool char_check = 0;
    if (BUFFER[i] == '(') {
      count++;
    } else if (BUFFER[i] == ')') {
      count--;
    } else {
      for (size_t j = 0; j <= strlen(ALLOWED_CHARS); j++) {
        if (BUFFER[i] == ALLOWED_CHARS[j]) {
          char_check = 1;
          break;
        }
      }
      if (!char_check) {
        printf("Wrong char {%c}, only alphabet in uppercase and ()\n",
               BUFFER[i]);
        exit(1);
      }
    }
  }
  if (count != 0) {
    printf("Wrong count of ()\n");
    exit(1);
  }

  obj = parse(BUFFER, 0, len_str + 1);

  printf("Введите булевы значения:\n");
  for (size_t i = 0; i < 30; i++) {
    if (alphabet[i] == 1) {
      printf("Значение для: %c = ", 'A' + (char)i);
      scanf("%i", &alphabet[i]);
    }
  }

  bool result = calc_result(obj, true);

  printf("\n\nРезультат: %b\n", result);

  free(obj);

  return EXIT_SUCCESS;
}
