#include <stdint.h>

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

extern float_class_t classify(double *value_ptr) {
    double *value_ptr_as_double = value_ptr;
    void *value_ptr_as_void = value_ptr_as_double;
    uint64_t *value_ptr_as_int = value_ptr_as_void;

    uint64_t sign = *value_ptr_as_int;
    sign >>= 63;

    uint64_t exp = *value_ptr_as_int;
    exp <<= 1;
    exp >>= 53;

    uint64_t man = *value_ptr_as_int;
    man <<= 12;
    man >>= 12;

    if (exp == 0) {
        if (man == 0) {
            return sign == 0 ? PlusZero : MinusZero;
        }

        return sign == 0 ? PlusDenormal : MinusDenormal;
    }

    if (exp == 2047) {
        if (man == 0) {
            return sign == 0 ? PlusInf : MinusInf;
        }

        return sign == 0 ? SignalingNaN : QuietNaN;
    }

    return sign == 0 ? PlusRegular : MinusRegular;
}