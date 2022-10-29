#include <stdio.h>

extern double avx_sin(double x);

int main() {
  double res = avx_sin(2.600);
  printf("%f\n", res);
  return 0;
}