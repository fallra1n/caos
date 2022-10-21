#include <stdio.h>
#include <stdlib.h>

extern void very_important_function(int N);
int *A;

int main() {
  int N = 3;
  A = malloc(N * 4);
  for (int i = 0; i < N; ++ i) {
    A[i] = i;
  }
  very_important_function(N);
  return 0;
}