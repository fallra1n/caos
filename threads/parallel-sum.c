#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ERROR_HANDLE(code, expected) if(code != expected) { return_val = 1; goto Exit;}
#define GUARD_SIZE 0
#define SUCCESS_RET_CODE 0

void *ParallelSum(void *arg) {
  int *partial_sum_ptr = arg;
  int cur_number;

  while (scanf("%d", &cur_number) == 1) {
    *partial_sum_ptr += cur_number;
  }

  return NULL;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    return 1;
  }

  int return_val = 0;

  int N = (int)strtol(argv[1], NULL, 10);

  pthread_t *threads = malloc(sizeof(pthread_t) * N);
  int *sums = malloc(sizeof(int) * N);
  memset(sums, 0, N * sizeof(int));
  pthread_attr_t thread_attrs;

  // set attributes
  int ret_code;
  ret_code = pthread_attr_init(&thread_attrs);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  ret_code = pthread_attr_setstacksize(&thread_attrs, PTHREAD_STACK_MIN);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  ret_code = pthread_attr_setguardsize(&thread_attrs, GUARD_SIZE);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  // create threads
  for (int i = 0; i < N; i++) {
    ret_code = pthread_create(&threads[i], &thread_attrs, ParallelSum, &sums[i]);
    ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)
  }

  // wait all threads
  for (int i = 0; i < N; i++) {
    ret_code = pthread_join(threads[i], NULL);
    ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)
  }

  int ans = 0;
  for (int i = 0; i < N; i++) {
    ans += sums[i];
  }

  printf("%d", ans);

Exit:
  pthread_attr_destroy(&thread_attrs);
  free(sums);
  free(threads);

  return return_val;
}