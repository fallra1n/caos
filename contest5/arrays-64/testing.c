#include <stdio.h>
#include <stdlib.h>

extern void sum(size_t N, const int *A, const int *B, int *R);

int main() {
  int N = 5;
  int *A = malloc(N * sizeof(int));
  int *B = malloc(N * sizeof(int));
  int *R = malloc(N * sizeof(int));
  
  for (int i = 0; i < N; ++i) {
    scanf("%d", &A[i]);
  }
  
  for (int i = 0; i < N; ++i) {
    scanf("%d", &B[i]);
  }

  sum(N, A, B, R);
  
  for (int i = 0; i < N; ++i) {
    printf("%d ", R[i]);  
  }

  free(A);
  free(B);
  free(R);
  return 0;
}