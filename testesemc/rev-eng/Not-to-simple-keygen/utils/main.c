#include "main.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

int randomData(const int min, const int max) {
    return (int)((rand() % (max - min + 1)) + min);

}

bytes_t hashUltraProXor(bytes_t src) {
    bytes_t out;
    out.len = 256;
    out.b = malloc(out.len);

    for (int i = 0; i + 3 < out.len; i++) {
        uint32_t s = (uint32_t)src.b[i % src.len];
        s = ((s << 13) ^ s) - (s >> 21);

        uint32_t n = s;
        n = (n * (n * n * 28732 + 910375) + 758127501);
        n += s;
        n = ((n << 17) ^ n) - (n >> 20);

        uint32_t x = n;
        x = (x * (x * 67912) + 7508275091);
        x += n;
        x = ((x << 19) ^ x) - (x >> 8);
        x += 0x9E3789B9;

        uint32_t y = x;
        y = (y * (y * y * 68992 + 120957) + 609182504);
        y ^= y >> 16;

        uint32_t z = y;
        z = (uint32_t)((z * 2654005761u) ^ z);

        uint32_t a = z;
        a = (a * (a * 0x9E3679F8u + a + (uint32_t)7095126095));

        uint32_t b = a;
        b = b * b * 0x9E3479B9u + b + 0x7F3A7C15u;
        b ^= b >> 28;

        b ^= b >> 16;
        b *= 0x85EBCA6Bu;
        b ^= b >> 13;
        b *= 0xC2B2AE35u;
        b ^= b >> 16;

        uint32_t v = b;

        out.b[i + 0] = (uint8_t)(v);
        out.b[i + 1] = (uint8_t)(v >> 8);
        out.b[i + 2] = (uint8_t)(v >> 16);
        out.b[i + 3] = (uint8_t)(v >> 24);

    }

    return out;

};

bytes_t hashUltraPro(bytes_t src) {
    #define HASH_LEN 256
    #define WORD_LEN 64
    #define MINI_WORD_LEN 8
    #define SEED_VAL 0xFF1FF2DD
    #define SHLR_SEED_VAL 27

    #define SEED_VAL_HELPER(x) ((x ^ ((SEED_VAL * x)) >> SHLR_SEED_VAL))

    bytes_t out;
    out.len = HASH_LEN;
    out.b = malloc(out.len);
    memset(out.b, 0xff, out.len);

    uint32_t state[WORD_LEN] = {
        SEED_VAL_HELPER(0x243F6A88), SEED_VAL_HELPER(0x85AE08D3),
        SEED_VAL_HELPER(0x13198A2E), SEED_VAL_HELPER(0x03707344),
        SEED_VAL_HELPER(0xA4093822), SEED_VAL_HELPER(0x299F31D0),
        SEED_VAL_HELPER(0x082EFA98), SEED_VAL_HELPER(0xEC4E6C89),
        SEED_VAL_HELPER(0x243F6D88), SEED_VAL_HELPER(0x85A308D3),
        SEED_VAL_HELPER(0x13198A2E), SEED_VAL_HELPER(0x03707344),
        SEED_VAL_HELPER(0xA4093822), SEED_VAL_HELPER(0x299F31D0),
        SEED_VAL_HELPER(0x082EFA98), SEED_VAL_HELPER(0xEC4E6C89),
        SEED_VAL_HELPER(0x243F6A88), SEED_VAL_HELPER(0x85A308D3),
        SEED_VAL_HELPER(0x13198A2E), SEED_VAL_HELPER(0x03707344),
        SEED_VAL_HELPER(0xA4093822), SEED_VAL_HELPER(0x299F31D0),
        SEED_VAL_HELPER(0x082EFA98), SEED_VAL_HELPER(0xEC4E6C89),
        SEED_VAL_HELPER(0x243F6A88), SEED_VAL_HELPER(0x85A308D3),
        SEED_VAL_HELPER(0x13198AFE), SEED_VAL_HELPER(0x03707344),
        SEED_VAL_HELPER(0xA4093822), SEED_VAL_HELPER(0x299F31D0),
        SEED_VAL_HELPER(0x082EFA98), SEED_VAL_HELPER(0xEC4E6C89),
        SEED_VAL_HELPER(0x243F6A88), SEED_VAL_HELPER(0x85A308D3),
        SEED_VAL_HELPER(0x13198A2E), SEED_VAL_HELPER(0x03707344),
        SEED_VAL_HELPER(0xA4093822), SEED_VAL_HELPER(0x299F31D0),
        SEED_VAL_HELPER(0x082EFA98), SEED_VAL_HELPER(0xEC4E6C89),
        SEED_VAL_HELPER(0x243F6A88), SEED_VAL_HELPER(0xFFFFF7FF),
        SEED_VAL_HELPER(0x13198A2E), SEED_VAL_HELPER(0x03707344),
        SEED_VAL_HELPER(0xA4093822), SEED_VAL_HELPER(0x299F31D0),
        SEED_VAL_HELPER(0x082EFA98), SEED_VAL_HELPER(0xEC476C89),
        SEED_VAL_HELPER(0x243F6A88), SEED_VAL_HELPER(0x85A308D3),
        SEED_VAL_HELPER(0x13198A2E), SEED_VAL_HELPER(0x03707344),
        SEED_VAL_HELPER(0xA4093822), SEED_VAL_HELPER(0x299F31D0),
        SEED_VAL_HELPER(0x082EFA98), SEED_VAL_HELPER(0xEC4E6C89),
        SEED_VAL_HELPER(0x243F6A88), SEED_VAL_HELPER(0x85B308D3),
        SEED_VAL_HELPER(0x13198A2E), SEED_VAL_HELPER(0x03707344),
        SEED_VAL_HELPER(0xA4093822), SEED_VAL_HELPER(0x299F31D0),
        SEED_VAL_HELPER(0x082EFA98), SEED_VAL_HELPER(0xEC4E6C89),

    };

    for (size_t i = 0; i < src.len; i++) {
        uint32_t in = src.b[i];

        uint32_t v =
            state[i % WORD_LEN]
            ^ (in * SEED_VAL_HELPER(0x1000193))
            ^ (state[(i + 3) % WORD_LEN] >> (i & (WORD_LEN - 1)))
            ^ SEED_VAL_HELPER(0xBCBDA371);
        size_t idx = (state[i % WORD_LEN] ^ (v * SEED_VAL_HELPER(0x9E3779B1))) & (WORD_LEN - 1);

        state[idx] ^= v;
        state[idx] *= SEED_VAL_HELPER(0x9E3779B1);
        state[idx] = (state[idx] << 13) | (state[idx] >> 19);

        state[(idx + 1) & (WORD_LEN - 1)] += state[idx];

    }

    for (int r = 0; r < MINI_WORD_LEN; r++) {
        for (int i = 0; i < WORD_LEN; i++) {
            state[i] ^= state[(i + r + 1) & 7];
            state[i] *= SEED_VAL_HELPER(0x85EBCA6B);
            state[i] ^= state[i] >> 13;

        }
    }

    for (int r = 0; r < (MINI_WORD_LEN * 2); r++) {
        for (int i = 0; i < WORD_LEN; i++) {
            state[i] ^= state[(i + 1) % WORD_LEN];
            state[i] *= SEED_VAL_HELPER(0x85EBCA6B);
            state[i] ^= state[i] >> 16;
            state[i] *= SEED_VAL_HELPER(0x7FEB352D);
            state[i] ^= state[i] >> 15;
        }
    }

    for (int i = 0; i < WORD_LEN; i++) {
        uint32_t v = state[i % WORD_LEN] ^ (i * SEED_VAL_HELPER(0x9E3779B9));
        out.b[i * 4 + 0] = (uint8_t)(v);
        out.b[i * 4 + 1] = (uint8_t)(v >> 8);
        out.b[i * 4 + 2] = (uint8_t)(v >> 16);
        out.b[i * 4 + 3] = (uint8_t)(v >> 24);

    }

    return out;

}

bytes_t xor(bytes_t src, bytes_t key) {
    uchar_t *srcCopy = malloc(src.len);
    memcpy(srcCopy, src.b, src.len);

    uchar_t *keyCopy = malloc(key.len);
    memcpy(keyCopy, key.b, key.len);

    uchar_t out[src.len];
    for (int i = 0; i < src.len; i++) {
        out[i] = srcCopy[i] ^ keyCopy[(i + 1) % key.len];
        keyCopy[(i + 2) % key.len] = keyCopy[i % key.len];

    }

    bytes_t output = {
        .len = src.len,
        .b = malloc(src.len)

    };

    memcpy(output.b, out, src.len);
    free(srcCopy);
    free(keyCopy);

    return output;

}

bytes_t XorEncode(bytes_t src) {
    uchar_t nonceRaw[NONCE_SIZE];
    for (int i = 0; i < NONCE_SIZE; i++) {
        nonceRaw[i] = randomData(0, 255);

    }

    bytes_t nonce = {
        .b = malloc(NONCE_SIZE),
        .len = NONCE_SIZE

    };

    memcpy(nonce.b, nonceRaw, (size_t)NONCE_SIZE);

    bytes_t hash = hashUltraProXor(nonce);
    bytes_t xorOut = xor(src, hash);
    bytes_t out = {
        .len = NONCE_SIZE + src.len

    };

    out.b = malloc(out.len);
    memcpy(out.b, nonce.b, nonce.len);
    memcpy(out.b + nonce.len, xorOut.b, xorOut.len);

    free(hash.b);
    free(nonce.b);
    free(xorOut.b);

    return out;

};

bytes_t XorDecode(bytes_t src) {
    bytes_t srcCopy = {
        .len = src.len,
        .b = malloc(src.len)

    };

    memcpy(srcCopy.b, src.b, srcCopy.len);

    bytes_t nonce;
    nonce.len = NONCE_SIZE;
    nonce .b = malloc(NONCE_SIZE);
    memcpy(nonce.b, srcCopy.b, NONCE_SIZE);

    bytes_t srcWithoutXor;
    srcWithoutXor.len = srcCopy.len - NONCE_SIZE;
    srcWithoutXor.b = malloc(srcWithoutXor.len);
    memcpy(srcWithoutXor.b, srcCopy.b + NONCE_SIZE, srcWithoutXor.len);

    bytes_t hash = hashUltraProXor(nonce);
    bytes_t unxor = xor(srcWithoutXor, hash);

    bytes_t out = {
        .len = unxor.len,
        .b = malloc(unxor.len)

    };

    memcpy(out.b, unxor.b, out.len);

    free(srcCopy.b);
    free(nonce.b);
    free(srcWithoutXor.b);
    free(hash.b);
    free(unxor.b);

    return out;

};

const encoder BestXorEncoder = {
    .encode = XorEncode,
    .decode = XorDecode
};

void bytesf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    for (const char *p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;

        }

        if (*(p + 1) == 'b') {
            bytes_t *bytes = va_arg(args, bytes_t*);
            if (*(p + 2) == 'h') {
                printf("[%#x", bytes->b[0]);
                for (int i = 1; i < bytes->len; i++) {
                    printf(", %#x", bytes->b[i]);

                }

                printf("]");
                fflush(stdout);
                p += 2;
                continue;

            }


            printf("[%x", bytes->b[0]);
            for (int i = 1; i < bytes->len; i++) {
                printf(", %x", bytes->b[i]);

            }

            printf("]");
            fflush(stdout);
            p++;
            continue;

        } else if (*(p + 1) == 'p') {
            void *ptr = va_arg(args, void*);
            printf("%p", ptr);
            fflush(stdout);
            p++;
            continue;

        } else if (*(p + 1) == '%') {
            printf("%%");
            fflush(stdout);
            p++;
            continue;

        }

    }

}
