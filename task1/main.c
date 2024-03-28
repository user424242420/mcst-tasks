#include <alloca.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>

struct run_merge_sort {
  int *up;
  int *down;
  unsigned int left;
  unsigned int right;
};

pthread_mutex_t mutex_count_threads = PTHREAD_MUTEX_INITIALIZER;
int count_threads, count_processes;
void *work(void *param);
void *merge_sort(void *arg);

unsigned int count_el;

int main(int argc, char *argv[]) {
  count_processes = get_nprocs_conf();

  int count;
  int *up;
  int *down;
  int *result;

  printf("Input count elements: ");
  scanf("%d", &count_el);
  up = malloc(count_el * sizeof(int));
  down = malloc(count_el * sizeof(int));

  for (unsigned int i = 0; i < count_el; i++) {
    scanf("%d", &up[i]);
  }
  struct run_merge_sort params = {up, down, 0, count_el};
  result = (int *)merge_sort((void *)&params);
  printf("\nResult:\n");
  for (unsigned int i = 0; i < count_el; i++) {
    printf("%d ", result[i + 1]);
  }
  printf("\n");

  return EXIT_SUCCESS;
}

void *merge_sort(void *arg) {

  struct run_merge_sort *decode;
  decode = (struct run_merge_sort *)arg;

  int *up = decode->up;
  int *down = decode->down;
  unsigned int left = decode->left;
  unsigned int right = decode->right;
  if (left == right) {
    down[left] = up[left];
    return down;
  }
  int *l_buff;
  int *r_buff;
  void *l_buff_v;
  void *r_buff_v;

  pthread_t thread1, thread2;
  int iret1, iret2;

  unsigned int middle = left + (right - left) / 2;
  struct run_merge_sort i1, i2;
  i1.up = up;
  i1.down = down;
  i1.left = left;
  i1.right = middle;

  i2.up = up;
  i2.down = down;
  i2.left = middle + 1;
  i2.right = right;

  if (count_threads > count_processes + 1) {

    l_buff = (int *)merge_sort((void *)&i1);
    r_buff = (int *)merge_sort((void *)&i2);
  }

  else {
    pthread_mutex_lock(&mutex_count_threads);
    count_threads += 2;
    pthread_mutex_unlock(&mutex_count_threads);
    iret1 = pthread_create(&thread1, NULL, merge_sort, (void *)&i1);
    iret2 = pthread_create(&thread2, NULL, merge_sort, (void *)&i2);
    pthread_join(thread1, &l_buff_v);
    pthread_join(thread2, &r_buff_v);
    pthread_mutex_lock(&mutex_count_threads);
    count_threads -= 2;
    pthread_mutex_unlock(&mutex_count_threads);
    l_buff = (int *)l_buff_v;
    r_buff = (int *)r_buff_v;
  }

  int *target = l_buff == up ? down : up;

  unsigned int l_cur = left, r_cur = middle + 1;
  for (unsigned int i = left; i <= right; i++) {
    if (l_cur <= middle && r_cur <= right) {
      if (l_buff[l_cur] < r_buff[r_cur]) {
        target[i] = l_buff[l_cur];
        l_cur++;
      } else {
        target[i] = r_buff[r_cur];
        r_cur++;
      }
    } else if (l_cur <= middle) {
      target[i] = l_buff[l_cur];
      l_cur++;
    } else {
      target[i] = r_buff[r_cur];
      r_cur++;
    }
  }

  return (void *)target;
}
