#include <stdio.h>
#include <stdlib.h>

extern void memset(int *ptr, int value, int size);

int main() {
  int *ptr = (int*)malloc(4 * sizeof(int));
  memset(ptr, 1, 4);

  for (int i = 0; i < 4; ++i) {
    printf("%d\n", *(ptr + i));
  }
}