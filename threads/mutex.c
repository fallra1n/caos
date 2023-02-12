#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <limits.h>

#define ERROR_HANDLE(code, expected) if (code != expected) { return_val = 1; goto Exit; }
#define GUARD_SIZE 0
#define SUCCESS_RET_CODE 0

#define LEFT_ADD 0.99
#define RIGHT_ADD 1.01
#define ME_ADD 1.0

typedef struct ThreadInfo {
  int id;
  int k;
  int N;
  double *data;
  pthread_mutex_t *mutex;
} ThreadInfo;

void *ThreadFunc(void *arg) {
  ThreadInfo *args = arg;
  int my_id = args->id;

  int left_id = (my_id == 0 ? args->k - 1 : my_id - 1);
  int right_id = (my_id == (args->k - 1) ? 0 : my_id + 1);

  for (int i = 0; i < args->N; i++) {
    pthread_mutex_lock(args->mutex);
    args->data[left_id] += LEFT_ADD;
    args->data[right_id] += RIGHT_ADD;
    args->data[my_id] += ME_ADD;
    pthread_mutex_unlock(args->mutex);
  }

  return NULL;
}



int main(int argc, char **argv) {
  if (argc < 3) {
    return 1;
  }

  int return_val = 0;

  int N = (int)strtol(argv[1], NULL, 10);
  int k = (int)strtol(argv[2], NULL, 10);

  pthread_t *threads = malloc(k * sizeof(pthread_t));
  double *nums = malloc(k * sizeof(double));
  memset(nums, 0, k * sizeof(double));

  ThreadInfo *thread_args = malloc(k * sizeof(ThreadInfo));

  // set attributes
  pthread_attr_t thread_attrs;

  int ret_code;
  ret_code = pthread_attr_init(&thread_attrs);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  ret_code = pthread_attr_setstacksize(&thread_attrs, PTHREAD_STACK_MIN);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  ret_code = pthread_attr_setguardsize(&thread_attrs, GUARD_SIZE);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  for (int i = 0; i < k; i++) {
    thread_args[i].k = k;
    thread_args[i].N = N;
    thread_args[i].id = i;
    thread_args[i].data = nums;
    thread_args[i].mutex = &mutex;
    ret_code = pthread_create(&threads[i], &thread_attrs, ThreadFunc, &thread_args[i]);
    ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)
  }

  // wait all threads
  for (int i = 0; i < k; i++) {
    ret_code = pthread_join(threads[i], NULL);
    ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)
  }

  for (int i = 0; i < k; i++) {
    printf("%.10g\n", nums[i]);
  }

Exit:
  free(nums);
  free(threads);
  free(thread_args);
  pthread_mutex_destroy( &mutex);
  pthread_attr_destroy(&thread_attrs);

  return return_val;
}