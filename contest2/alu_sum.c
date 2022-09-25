#include <stdint.h>
#include <stdio.h>

typedef uint16_t ITYPE;

extern void sum(ITYPE first, ITYPE second, ITYPE *res, int *CF) {
	ITYPE xor_bit = first ^ second;
	ITYPE and_bit = (first & second);
	and_bit <<= 1;
	ITYPE temp = first;

	while (and_bit > 0) {
		temp = xor_bit;
		xor_bit = temp ^ and_bit;
		and_bit = temp & and_bit;
		and_bit <<= 1;
	}

	*res = xor_bit;
	if (xor_bit < first || xor_bit < second) {
		*CF = 1;
	} else {
		*CF = 0;
	}

	printf("%u\n", xor_bit);
	printf("%d\n", *CF);
}

int main() {
	uint16_t res = 0;
	int CF = 12;
	sum(3, 5, &res, &CF);
}