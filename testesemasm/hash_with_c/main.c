#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct {
    unsigned char *b;
    size_t len;
    
} bytes_t;

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

bytes_t hash(bytes_t);

int main() {
    bytes_t source = {
        .b = (unsigned char*)"abc",
        .len = 4
        
    };
    
    printf("Original source: %s\n", (char*)source.b);
    bytesf("Original source (bytes): %bh\n", &source);
    bytes_t hashed = hash(source);
    printf("Hashed .len: %d\n", (int)hashed.len);
    bytesf("Hash: %bh\n", &hashed);
    
    return 0;
    
}