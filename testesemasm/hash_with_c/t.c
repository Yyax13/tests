#include <stddef.h>

typedef struct {
    unsigned char *b;
    size_t len;
    
} bytes_t;

bytes_t ret(bytes_t bytes) {
    bytes_t out;
    out.b = bytes.b;
    out.len = bytes.len;
    
    return out;
    
};

void nothing() {
    
}

int main() {
    bytes_t bytes = {
        .b = (unsigned char*)"abc123",
        .len = 7
        
    };
    
    bytes_t bytes2 = ret(bytes);
    nothing();
    return 0;
    
};