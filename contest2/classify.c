#include <stdint.h>

const int8_t AMOUNT_OF_MAN_AND_EXP_BITS = 63;
const int8_t AMOUNT_OF_SIGN_AND_EXP_BITS = 12;
const int8_t AMOUNT_OF_SIGN_BITS = 1;
const int8_t AMOUNT_OF_MAN_BITS = 52;

typedef enum {
    PlusZero = 0x00,
    MinusZero = 0x01,
    PlusInf = 0xF0,
    MinusInf = 0xF1,
    PlusRegular = 0x10,
    MinusRegular = 0x11,
    PlusDenormal = 0x20,
    MinusDenormal = 0x21,
    SignalingNaN = 0x30,
    QuietNaN = 0x31
} float_class_t;

float_class_t classify(double *value_ptr) {
    int64_t result;

    double *value_ptr_as_double = value_ptr;
    void *value_ptr_as_void = value_ptr_as_double;
    uint64_t *value_ptr_as_int = value_ptr_as_void;

    uint64_t sign = *value_ptr_as_int;
    sign >>= AMOUNT_OF_MAN_AND_EXP_BITS;

    uint64_t exp = *value_ptr_as_int;
    exp <<= AMOUNT_OF_SIGN_BITS;
    exp >>= AMOUNT_OF_MAN_BITS + 1;

    uint64_t man = *value_ptr_as_int;
    man <<= AMOUNT_OF_SIGN_AND_EXP_BITS;
    man >>= AMOUNT_OF_SIGN_AND_EXP_BITS;

    if (exp == 0) {
        if (man == 0) {
            result = (sign == 0 ? PlusZero : MinusZero);
        } else {
            result = (sign == 0 ? PlusDenormal : MinusDenormal);
        }
    } else if (exp == 2047) {
        if (man == 0) {
            result = (sign == 0 ? PlusInf : MinusInf);
        } else {
            result = (sign == 0 ? SignalingNaN : QuietNaN);
        }
    } else {
        result = (sign == 0 ? PlusRegular : MinusRegular);
    }

    return result;
}