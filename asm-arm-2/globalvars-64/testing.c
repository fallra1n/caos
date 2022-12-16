#include <stdio.h>
#include <stdint.h>

int64_t A = 1;
int64_t B = 2;
int64_t C = 3;
int64_t D = 4;

extern int64_t R;
extern void calculate();

int main() {
  calculate();
  printf("%lld", R);
  return 0;
}