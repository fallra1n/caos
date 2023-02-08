#include <stdio.h>
#include <pthread.h>

void *ReverseNumbers(void *args) {
  int cur_number;
  int scanf_ret_val = scanf("%d", &cur_number);
  if (scanf_ret_val != 1) {
    return NULL;
  }

  pthread_t next_thread_id;
  pthread_create(&next_thread_id, NULL, ReverseNumbers, NULL);

  pthread_join(next_thread_id, NULL);

  printf("%d\n", cur_number);
  return NULL;
}

int main() {
  ReverseNumbers(NULL);
  return 0;
}