#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

#define ERROR_HANDLE(code, expected) if (code != expected) { return_val = 1; goto Exit; }
#define SUCCESS_RET_CODE 0

typedef struct ThreadInfo {
  int64_t A;
  int64_t B;
  int32_t N;
  int64_t *data;
  int64_t *searched_count;
  pthread_mutex_t *mutex;
  pthread_cond_t *cond_var;
} ThreadInfo;

bool IsPrime(int64_t num) {
  for (int64_t i = 2; i * i <= num; i++) {
    if (num % i == 0) {
      return false;
    }
  }

  return true;
}

void *SearchSimpleNums(void *arg) {
  ThreadInfo *args = arg;

  for (int64_t i = args->A; i <= args->B; i++) {
    if (!IsPrime(i)) {
      continue;
    }

    pthread_mutex_lock(args->mutex);

    args->data[(*(args->searched_count))++] = i;
    pthread_cond_signal(args->cond_var);

    pthread_mutex_unlock(args->mutex);

    if (*(args->searched_count) == args->N) {
      break;
    }
  }

  return NULL;
}


int main(int argc, char **argv) {
  if (argc < 4) {
    return 1;
  }

  int return_val = 0;

  int64_t A = strtol(argv[1], NULL, 10);
  int64_t B = strtol(argv[2], NULL, 10);
  int32_t N = (int32_t)strtol(argv[3], NULL, 10);

  pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  int64_t *data = malloc(N * sizeof(int64_t));

  int64_t searched_count = 0;
  ThreadInfo args = {A, B, N, data, &searched_count, &mutex, &cond_var};
  pthread_t second_thread;

  int ret_code = pthread_create(&second_thread, NULL, SearchSimpleNums, &args);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  int32_t count = 0;
  while (count < N) {
    pthread_mutex_lock(&mutex);

    pthread_cond_wait(&cond_var, &mutex);

    for (; count < searched_count; count++) {
      printf("%ld\n", data[count]);
    }

    pthread_mutex_unlock(&mutex);
  }

Exit:
  pthread_cond_destroy(&cond_var);
  return_val = pthread_mutex_destroy(&mutex);
  free(data);

  return return_val;
}