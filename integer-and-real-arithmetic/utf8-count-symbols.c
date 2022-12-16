#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

const uint8_t END_OF_ASCII = 127;
const uint8_t ASCII_FIRST_BIT = 0;
const uint8_t GET_FIRST_BIT = 7;
const uint8_t GET_FIRST_TWO_BITS = 6;
const uint8_t MINOR_BYTES_HEADER = 2;
const uint8_t MAIN_BYTE_HEADER_BITS = 1;

struct AmountOfSymbols {
  int32_t fits_under_utf8;
  int32_t unsuitable_under_utf8;
};

bool MatchesTheUTF8(uint8_t symbol_as_int) {
  uint8_t is_crush = false;
  uint8_t amount_of_bytes = 0;
  
  int32_t i = 0;
  while (symbol_as_int >> (GET_FIRST_BIT - i) == MAIN_BYTE_HEADER_BITS) {
    ++amount_of_bytes;
    ++i;
  }

  char temp;
  for (i = 0; i < amount_of_bytes; ++i) {
    if ((temp = getchar()) == EOF || (uint8_t)temp >> GET_FIRST_TWO_BITS != MINOR_BYTES_HEADER) {
      is_crush = true;
      break;
    }
  }

  return !is_crush;
}

bool MatchesTheASCII(uint8_t symbol_as_int) {
  return symbol_as_int >> GET_FIRST_BIT == ASCII_FIRST_BIT;
}

bool ClassyfySymbol(struct AmountOfSymbols *result, char symbol) {
  bool is_crush = false;

  if (MatchesTheASCII((uint8_t)symbol)) {
    ++(result->fits_under_utf8);
  } else {
    bool is_correct = MatchesTheUTF8((uint8_t)symbol);
    if (is_correct) {
      ++(result->unsuitable_under_utf8);
    } else {
      is_crush = true;
    }
  }

  return is_crush;
}

void SequenceHandler(struct AmountOfSymbols *result) {
  bool is_crush = false;
  char symbol;

  while ((symbol = getchar()) != EOF && !is_crush) {    
    is_crush = ClassyfySymbol(result, symbol);
  }
}

int main() {
  struct AmountOfSymbols result = {0, 0};
  SequenceHandler(&result);
  printf("\n%d %d\n", result.fits_under_utf8, result.unsuitable_under_utf8);
  return 0;
}