#include <stdio.h>
#include <stdint.h>
                                          //63=0 62=0 61 ... 36   35 ... 10  9 ... 0 - bits of int64_t
                                          //           |     |    |      |   |     |
const int8_t CARDINALITY_OF_THE_SET = 62; //           z ... a    Z ...  A   9 ... 0 - our set 
const int8_t END_OF_DIGITS = 10;          //           |     |    |      |   |     |
const int8_t END_OF_UPPERCASE = 36;       //          62 ... 37  36 ... 11  10 ... 1
const uint64_t TWO_TO_THE_32_DEGREE = (1ULL << 62) - 1;

void ExpressionProcessing(uint64_t *base, uint64_t *cur) {
    char temp_char;
    int a = scanf("%c", &temp_char);
    uint64_t serial_number = 1;

    while (a > 0 && (temp_char != '\0' && temp_char != '\n')) {
        switch (temp_char) {
            case '&':
                *base &= *cur;
                *cur = 0;
                break;
            case '|':
                *base |= *cur;
                *cur = 0;
                break;
            case '^':
                *base ^= *cur;
                *cur = 0;
                break;
            case '~':
                *base ^= TWO_TO_THE_32_DEGREE;
                *cur = 0;
                break;
            default:
                serial_number = 1;
                if (temp_char >= '0' && temp_char <= '9') {
                    serial_number <<= (int) temp_char - '0';
                }
                if (temp_char >= 'A' && temp_char <= 'Z') {
                    serial_number <<= (int) temp_char - 'A' + END_OF_DIGITS;
                }
                if (temp_char >= 'a' && temp_char <= 'z') {
                    serial_number <<= (int) temp_char - 'a' + END_OF_UPPERCASE;
                }

                *cur |= serial_number;
        }
        a = scanf("%c", &temp_char);
    }
}

void PrintTheFinalSet(uint64_t *base) {
    int8_t i = 1;
    while (*base != 0 && i <= CARDINALITY_OF_THE_SET) {
        if (*base % 2 == 1) {
            if (i >= 1 && i <= END_OF_DIGITS) {
                printf("%c", (char) (i - 1 + '0'));
            } else if (i >= END_OF_DIGITS + 1 && i <= END_OF_UPPERCASE) {
                printf("%c", (char) (i - 1 - END_OF_DIGITS + 'A'));
            } else {
                printf("%c", (char) (i - 1 - END_OF_UPPERCASE + 'a'));
            }
        }
        *base >>= 1;
        ++i;
    }
}

int main() {
    uint64_t base = 0;
    uint64_t cur = 0;
    ExpressionProcessing(&base, &cur);
    PrintTheFinalSet(&base);
    return 0;
}