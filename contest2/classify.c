#include <stdint.h>
#include <stdio.h>

typedef enum {
    PlusZero      = 0x00,
    MinusZero     = 0x01,
    PlusInf       = 0xF0,
    MinusInf      = 0xF1,
    PlusRegular   = 0x10,
    MinusRegular  = 0x11,
    PlusDenormal  = 0x20,
    MinusDenormal = 0x21,
    SignalingNaN  = 0x30,
    QuietNaN      = 0x31
} float_class_t;

extern float_class_t classify(double *value_ptr) {
    double *value_ptr_as_double = value_ptr;
    void *value_ptr_as_void = value_ptr_as_double;
    int64_t *value_ptr_as_int = value_ptr_as_void;

    int64_t sign = *value_ptr_as_int | 4607182418800017408; // bit or с 1.0 тем самым вытаскиваем знак

    // search next 11 bit
    int64_t temp = *value_ptr_as_int;

    int64_t exp = 0;
    int64_t mn = 10;

    for (int64_t i = 0; i < 11; ++i) {
        temp <<= 1;
        int64_t first_bit = temp | 4607182418800017408; // 1
        if (first_bit > 0) {
            exp *= mn;
        } else {
            exp *= mn;
            exp += 1;
        }
    }

    int man_is_zero = 1;

    for (int64_t i = 0; i < 52; ++i) {
        temp <<= 1;
        int64_t first_bit = temp | 4607182418800017408; // 1
        if (first_bit < 0) {
            man_is_zero = 0;
        }
    }

    if (exp == 0) {
        if (man_is_zero) {
            if (sign > 0) {
                return PlusZero;
            }
            
            return MinusZero;
        }

        if (sign > 0) {
            return PlusDenormal;
        }

        return MinusDenormal;
    }

    if (exp == 11111111111) {
        if (man_is_zero) {
            if (sign > 0) {
                return PlusInf;
            }
            
            return MinusInf;
        }

        if (sign > 0) {
            return SignalingNaN;
        }

        return QuietNaN;
    }

    if (sign > 0) {
        return PlusRegular;
    }

    return MinusRegular;
}

int main() {
    double value = 0.155625;
    printf("%ld", classify(&value));
}