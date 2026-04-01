#pragma once

#include <stdlib.h>

#define NONCE_SIZE 128

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long ulong;

typedef struct {
    uchar    *b;
    size_t  len;
    
} bytes;

typedef struct {
    bytes (*encode)(bytes src);
    bytes (*decode)(bytes src);
    
} encoder;

void bprintf(const char *fmt, ...);
int prandom(int min, int max);

// Real Random Reluzent Resilient Riced Xor
extern const encoder rrrrrxor;