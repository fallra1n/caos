#include <stdio.h>
#include <stdlib.h>

extern double very_important_function(size_t N, const float *A, const float *B, float *R);

int main() {
  size_t N = 4;
  float *A = aligned_alloc(32, N * sizeof(float));
  float *B = aligned_alloc(32, N * sizeof(float));
  float *R = aligned_alloc(32, N * sizeof(float));

  for (int i = 0; i < N; ++i) {
    scanf("%f", &A[i]);
  }
  
  for (int i = 0; i < N; ++i) {
    scanf("%f", &B[i]);
  }

  double res = very_important_function(N, A, B, R);
  
  for (int i = 0; i < N; ++i) {
    printf("%f ", R[i]);  
  }

  printf("=========\n%f", res);

  free(A);
  free(B);
  free(R);
  return 0;
}