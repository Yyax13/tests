#pragma once

#include "../main.h"

#define NONCE_SIZE 256

typedef struct {
    bytes_t (*encode)(bytes_t src);
    bytes_t (*decode)(bytes_t src);

} encoder;

extern const encoder BestXorEncoder;

bytes_t hashUltraPro(bytes_t src);
void bytesf(const char *fmt, ...);