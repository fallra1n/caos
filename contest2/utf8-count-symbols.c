#include <stdio.h>
#include <stdint.h>

int main() {
  
  int fit = 0;
  int unsuitable = 0;

  char symbol;
  int cnt = scanf("%c", &symbol);

  while (symbol != EOF && cnt > 0) {
    uint32_t symbol_as_int = (int)symbol;
    uint32_t first = symbol_as_int >> 24;
    uint32_t second = symbol_as_int << 8 >> 24;
    uint32_t third = symbol_as_int << 16 >> 24;
    uint32_t fourth = symbol_as_int << 24 >> 24;

    if (first >> 3 == 30) { // == 11110
      if (second >> 6 != 2 || third >> 6 != 2 || fourth >> 6 != 2) {
        printf("%d %d", fit, unsuitable);
        return 1;
      }
      ++unsuitable;
      cnt = scanf("%c", &symbol);
      continue;
    }

    if (second >> 4 == 14) { // == 1110
      if (first != 0 || third >> 6 != 2 || fourth >> 6 != 2) {
        printf("%d %d", fit, unsuitable);
        return 1;
      }
      ++unsuitable;
      cnt = scanf("%c", &symbol);
      continue;
    }

    if (third >> 5 == 6) { // == 110
      if (first != 0 || second != 0 || fourth >> 6 != 2) {
        printf("%d %d", fit, unsuitable);
        return 1;
      }
      ++unsuitable;
      cnt = scanf("%c", &symbol);
      continue;
    }

    if (fourth >> 7 != 0) {
      printf("%d %d", fit, unsuitable);
      return 1;
    }

    if (first != 0 || second != 0 || third != 0) {
      printf("%d %d", fit, unsuitable);
      return 1;
    }

    ++fit;
    cnt = scanf("%c", &symbol);
  }
  
  printf("%d %d", fit, unsuitable);
  return 0;
}