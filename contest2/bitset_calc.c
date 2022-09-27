#include <stdio.h>
#include <stdint.h>

void ExpressionProcessing(int64_t *base, int64_t *cur, int8_t end_of_digits, int8_t end_of_uppercase) {
    char temp_char;
    int a = scanf("%c", &temp_char);
    int64_t serial_number = 1;

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
                *base ^= 4611686018427387903; // xor with 2^62 - 1
                *cur = 0;
                break;
            default:
                serial_number = 1;
                if (temp_char >= '0' && temp_char <= '9') {
                    serial_number <<= (int)temp_char - '0';
                }
                if (temp_char >= 'A' && temp_char <= 'Z') {
                    serial_number <<= (int)temp_char - 'A' + end_of_digits;
                }
                if (temp_char >= 'a' && temp_char <= 'z') {
                    serial_number <<= (int)temp_char - 'a' + end_of_uppercase;
                }

                *cur |= serial_number;
        }
        a = scanf("%c", &temp_char);
    }
}

void PrintTheFinalSet(int64_t *base, int8_t end_of_digits, int8_t end_of_uppercase, int8_t cardinality_of_the_set) {
    int8_t i = 1;
    while (*base != 0 && i <= cardinality_of_the_set) {
        if (*base % 2 == 1) {
            if (i >= 1 && i <= end_of_digits) {
                printf("%c", (char)(i - 1 + '0'));
            }
            else if (i >= end_of_digits + 1 && i <= end_of_uppercase) {
                printf("%c", (char)(i - 1 - end_of_digits + 'A'));
            }
            else {
                printf("%c", (char)(i - 1 - end_of_uppercase + 'a'));
            }
        }
        *base >>= 1;
        ++i;
    }
}

int main() {
    int64_t base = 0;
    int64_t cur = 0;
    int8_t cardinality_of_the_set = 62;
    int8_t end_of_digits = 10;
    int8_t end_of_uppercase = 36;
    ExpressionProcessing(&base, &cur, end_of_digits, end_of_uppercase);
    PrintTheFinalSet(&base, end_of_digits, end_of_uppercase, cardinality_of_the_set);
    return 0;
}