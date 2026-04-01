#pragma once

#include <stddef.h>

typedef unsigned long ulong_t;
typedef unsigned int uint_t;
typedef unsigned char uchar_t;
typedef unsigned short ushort_t;
typedef struct {
    uchar_t *b;
    size_t len;
} bytes_t;

typedef struct {
    char *s;
    size_t len;
} string_t;

typedef struct {
    string_t FullName;
    string_t Email;

} UserData_t;

typedef enum {
    FALSE = 0,
    TRUE = 1
} bool_t;

#include <stdint.h>

#define FAKE_THREAD_BEGIN() do { \
volatile int __spin = 0; \
for (volatile int __i = 0; __i < 1; __i++) { \
__spin += __i; \
}

#define FAKE_THREAD_END() \
} while (0)

static void obf_anchor_func(void) { }
#define OBF_INT(v) ({ \
    volatile int _x = (v); \
    volatile int _k = (int)(uintptr_t)&obf_anchor_func; \
    volatile int _y = _x ^ _k; \
    _y = (_y + _k) - _k; \
    _y = (_y ^ _k); \
    _y; \
})

#include <stdint.h>

#define OBF_HEX32(v) ({ \
volatile int _x = (int)(v); \
volatile int _k = (int)(uintptr_t)&obf_anchor_func; \
\
/* fake thread 1 */ \
FAKE_THREAD_BEGIN(); \
_x ^= _k; \
FAKE_THREAD_END(); \
\
/* fake thread 2 (ruído) */ \
FAKE_THREAD_BEGIN(); \
volatile int _n = _x ^ 0x5A5A5A5A; \
if (_n & 1) { \
_n += 2; \
_n -= 2; \
} else { \
_n ^= 0x11; \
_n ^= 0x11; \
} \
FAKE_THREAD_END(); \
\
/* fake thread 3 */ \
FAKE_THREAD_BEGIN(); \
_x = (_x * 9) - 18; \
FAKE_THREAD_END(); \
\
/* fake thread 4 */ \
FAKE_THREAD_BEGIN(); \
_x = (_x / 3) * 3; \
FAKE_THREAD_END(); \
\
/* fake thread 5 */ \
FAKE_THREAD_BEGIN(); \
_x = (_x + 18) / 9; \
FAKE_THREAD_END(); \
\
/* fake thread 6 */ \
FAKE_THREAD_BEGIN(); \
_x ^= _k; \
FAKE_THREAD_END(); \
\
_x; \
})

#include <stdint.h>

#define OBF_HEX8(v) ({ \
volatile uint8_t _x = (uint8_t)(v); \
volatile uint8_t _k = (uint8_t)(uintptr_t)&obf_anchor_func; \
\
/* camada ancora */ \
_x ^= _k; \
\
/* inferno algébrico 8-bit */ \
_x = (uint8_t)((_x * 5u) + 7u); \
_x = (uint8_t)((_x - 7u) / 5u); \
\
/* ruído inútil */ \
volatile uint8_t _n = _x ^ 0x5Au; \
if (_n & 1u) { \
_n += 2u; \
_n -= 2u; \
} else { \
_n ^= 0x11u; \
_n ^= 0x11u; \
} \
\
/* cancela ancora */ \
_x ^= _k; \
_x; \
})