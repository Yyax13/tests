#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"

void bprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    for (const char *p = fmt; *p; p++) {
        if (*p == '%' && *(p + 1) == 'b') {
            bytes val = va_arg(args, bytes);
            printf("[%#x", val.b[0]);
            for (int i = 1; i < val.len; i++) {
                printf(", %#x", val.b[i]);
                
            }
            printf("]");
            p++;
            
        } else {
            putchar(*p);
            
        }
        
    }
    
    va_end(args);
    
}

int prandom(int min, int max) {
    return (int)((rand() % (max - min + 1)) + min);
    
}

bytes rrxor(bytes src, bytes key) {
    bytes srcCopy = src;
    bytes keyCopy = key;
    uchar outB[srcCopy.len];

    for (int i = 0; i < (int)srcCopy.len; i++) {
        outB[i] = srcCopy.b[i] ^ keyCopy.b[(i+1)%keyCopy.len];
        keyCopy.b[(i+2)%keyCopy.len] = keyCopy.b[i%keyCopy.len];
        
    }
    
    bytes out = {
        .len = srcCopy.len,
        .b = malloc(srcCopy.len)
        
    };
    
    memcpy(out.b, outB, srcCopy.len);
    
    return out;
    
}

bytes djb2_hash(const unsigned char *data, size_t len) {
    unsigned long hash = 5381;

    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + data[i]; // hash * 33 + c
    }

    bytes out = {
        .len = sizeof(hash),
        .b = malloc(sizeof(hash))
        
    };
    
    memcpy(out.b, &hash, sizeof(hash));
    return out;
    
}

bytes xorEncode(bytes src) {
    srand(time(NULL));
    uchar nonce[NONCE_SIZE];
    for (int i = 0; i < NONCE_SIZE; i++) {
        nonce[i] = prandom(0, 255);
        
    }
    
    bytes hash = djb2_hash(nonce, NONCE_SIZE);
    bytes xor = rrxor(src, hash);
    bytes out;
    
    out.len = NONCE_SIZE + xor.len;
    out.b = malloc(out.len);
    
    memcpy(out.b, nonce, NONCE_SIZE);
    memcpy(out.b + NONCE_SIZE, xor.b, xor.len);
    
    free(xor.b);
    free(hash.b);
    
    return out;
    
};

bytes xorDecode(bytes src) {
    bytes nonce = {
        .len = NONCE_SIZE,
        .b = malloc(NONCE_SIZE)
        
    };
    
    memcpy(nonce.b, src.b, NONCE_SIZE);
    bytes hash = djb2_hash(nonce.b, NONCE_SIZE);
    
    bytes srcWithoutNonce = {
        .len = src.len - NONCE_SIZE,
        .b = (src.b + NONCE_SIZE)
    
    };
    
    bytes xor = rrxor(srcWithoutNonce, hash);
    return xor;
    
}

// Real Random Reluzent Resilient Riced Xor
const encoder rrrrrxor = {
    .encode = xorEncode,
    .decode = xorDecode
};
