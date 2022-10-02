#include <stdio.h>
#include <stdint.h>

const uint8_t END_OF_ASCII = 127;
const uint8_t ASCII_FIRST_BIT = 0;
const uint8_t GET_FIRST_BIT = 7;
const uint8_t GET_FIRST_TWO_BITS = 6;
const uint8_t MINOR_BITS_HEADER = 2;
const uint8_t FALSE = 0;
const uint8_t TRUE = 1;

struct AmountOfSymbols {
  int32_t fits_under_utf8;
  int32_t unsuitable_under_utf8;
};

int SymbolHandler(uint8_t symbol_as_int) {
  uint8_t is_crush = FALSE;
  uint8_t amount_of_bytes = 0;
  
  int32_t i = 0;
  while (symbol_as_int >> (GET_FIRST_BIT - i) == 1) {
    ++amount_of_bytes;
    ++i;
  }

  char temp;
  for (i = 0; i < amount_of_bytes; ++i) {
    if ((temp = getchar()) == EOF || (uint8_t)temp >> GET_FIRST_TWO_BITS != MINOR_BITS_HEADER) {
      is_crush = TRUE;
      break;
    }
  }

  return is_crush;
}

void SequenceHandler(struct AmountOfSymbols *result) {
  char symbol;

  while ((symbol = getchar()) != EOF) {    
    if ((uint8_t)symbol >> GET_FIRST_BIT == ASCII_FIRST_BIT) {
      ++(result->fits_under_utf8);
    } else {
      if (SymbolHandler((uint8_t)symbol)) {
        return;
      }
      ++(result->unsuitable_under_utf8);
    }
  }
}

int main() {
  struct AmountOfSymbols result = {0, 0};
  SequenceHandler(&result);
  printf("\n%d %d\n", result.fits_under_utf8, result.unsuitable_under_utf8);
  return 0;
}