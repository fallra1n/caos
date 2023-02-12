#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <sched.h>
#include <stdatomic.h>

#define ERROR_HANDLE(code, expected) if(code != expected) { return_val = 1; goto Exit;}
#define GUARD_SIZE 0
#define SUCCESS_RET_CODE 0

typedef struct Item {
  struct Item *next;
  int64_t value;
} item_t;

typedef struct ThreadInfo {
  int64_t id;
  int64_t k;
} thread_info_t;

_Atomic item_t *head;

void *ThreadFunc(void *arg) {
  thread_info_t *args = arg;
  int64_t id = args->id;
  int64_t k = args->k;

  for (int64_t i = id * k; i < (id + 1) * k; i++) {
    item_t* new = malloc(sizeof(item_t));
    new->value = i;
    new->next = NULL;
    void *temp = new;
    void *old = atomic_exchange(&(head), temp);
    new->next = old;
  }

  return NULL;
}

void Destroy(void *list) {
  item_t *h = list;

  while (h != NULL) {
    item_t *old = h;
    h = h->next;
    free(old);
  }
}

int main(int argc, char **argv) {
  if (argc < 3) {
    return 1;
  }

  int return_val = 0;

  int64_t N = (int64_t)strtol(argv[1], NULL, 10);
  int64_t k = (int64_t)strtol(argv[2], NULL, 10);

  pthread_t *threads = malloc(N * sizeof(pthread_t));

  int ret_code;

  atomic_store(&head, NULL);

  thread_info_t *threads_args = malloc(N * sizeof(thread_info_t));

  // set attributes
  pthread_attr_t thread_attrs;
  ret_code = pthread_attr_init(&thread_attrs);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  ret_code = pthread_attr_setstacksize(&thread_attrs, PTHREAD_STACK_MIN);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  ret_code = pthread_attr_setguardsize(&thread_attrs, GUARD_SIZE);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  for (int64_t i = 0; i < N; i++) {
    threads_args[i].id = i;
    threads_args[i].k = k;

    ret_code = pthread_create(&threads[i], &thread_attrs, ThreadFunc, &threads_args[i]);
    ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)
  }

  // wait all threads
  for (int64_t i = 0; i < N; i++) {
    ret_code = pthread_join(threads[i], NULL);
    ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)
  }

  void *res_s = atomic_load(&head);
  item_t *res = res_s;
  while (res != NULL) {
    printf("%ld\n", res->value);
    res = res->next;
  }

  Exit:
  free(threads);
  free(threads_args);
  pthread_attr_destroy(&thread_attrs);
  Destroy(atomic_load(&head));

  return return_val;
}