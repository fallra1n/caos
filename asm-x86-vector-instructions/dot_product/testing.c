#include <stdio.h>
#include <stdlib.h>

extern float dot_product(int N, const float *A, const float *B);

int main() {
  int N = 10;
  float *A = aligned_alloc(32, N * sizeof(float));
  float *B = aligned_alloc(32, N * sizeof(float));

  for (int i = 0; i < N; ++i) {
    scanf("%f", &A[i]);
  }
  
  for (int i = 0; i < N; ++i) {
    scanf("%f", &B[i]);
  }

  float res = dot_product(N, A, B);

  printf("%f", res);

  free(A);
  free(B);
  return 0;
}
