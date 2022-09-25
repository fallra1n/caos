#include <stdint.h>
#include <stdio.h>

typedef uint16_t ITYPE;

extern void mul(ITYPE first, ITYPE second, ITYPE *res, int *CF) {
    int flag = 0;
    *res = 0;

    for (ITYPE i = 1; i <= second; ++i) {
        ITYPE xor_bit = *res ^ first;
        ITYPE and_bit = *res & first;
        and_bit <<= 1;
        ITYPE temp = first;

        while (and_bit > 0)
        {
            temp = xor_bit;
            xor_bit = temp ^ and_bit;
            and_bit = temp & and_bit;
            and_bit <<= 1;
        }

        *res = xor_bit;

        if (!flag) {
            if (xor_bit < first || xor_bit < second) {
                flag = 1;
                *CF = 1;
            } else {
                *CF = 0;
            }
        }
    }
}

int main() {
    uint16_t res = 0;
	int CF = 12;
	mul(32000, 6, &res, &CF);
}