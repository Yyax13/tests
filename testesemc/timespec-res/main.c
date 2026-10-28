#include <stdio.h>
#include <time.h>

int main() {
    printf("== Timer resolution test\n\n");

    struct timespec ts;
    if (timespec_getres(&ts, TIME_UTC) == TIME_UTC) {
        printf("== Timer resolution (s): %ld.%09ld\n", ts.tv_sec, ts.tv_nsec);
        printf("== Timer resolution (ns): %ld\n", ts.tv_nsec);
    } else {
        printf("Can't get timer resolution");
        return 1;
    }

    return 0;
}
