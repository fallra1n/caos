#include <stdint.h>

const int8_t MAN_AND_EXP_BITS = 63;
const int8_t SIGN_AND_EXP_BITS = 12;
const int8_t SIGN_BITS = 1;
const int8_t MAN_BITS = 52;
const uint64_t TWO_TO_THE_11_DEGREE = (1ULL << 11);

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

void SetResult(int64_t *result, uint64_t man, uint64_t sign, int64_t p_first, int64_t m_first, int64_t p_second, int64_t m_second) {
    if (man == 0) {
        *result = (sign == 0 ? p_first : m_first);
    } else {
        *result = (sign == 0 ? p_second : m_second);
    }
}

float_class_t classify(double *value_ptr) {
    int64_t result;

    double *value_ptr_as_double = value_ptr;
    void *value_ptr_as_void = value_ptr_as_double;
    uint64_t *value_ptr_as_int = value_ptr_as_void;

    uint64_t sign = *value_ptr_as_int;
    sign >>= MAN_AND_EXP_BITS;

    uint64_t exp = *value_ptr_as_int;
    exp <<= SIGN_BITS;
    exp >>= MAN_BITS + 1;

    uint64_t man = *value_ptr_as_int;
    man <<= SIGN_AND_EXP_BITS;
    man >>= SIGN_AND_EXP_BITS;

    if (exp == 0) {
        SetResult(&result, man, sign, PlusZero, MinusZero, PlusDenormal, MinusDenormal);
    } else if (exp == TWO_TO_THE_11_DEGREE - 1) {
        SetResult(&result, man, sign, PlusInf, MinusInf, SignalingNaN, QuietNaN);
    } else {
        result = (sign == 0 ? PlusRegular : MinusRegular);
    }

    return result;
}