#include <stdio.h>
#include <threads.h>
#include <time.h>

int main(void) {
    mtx_t mu;
    int sensitive_variable = 1337;
    printf("Sensitive Variable: %d\n", sensitive_variable);

    if (mtx_init(&mu, mtx_plain | mtx_timed) != thrd_success) {
        printf("Can't init mutex\n");
        return 1;
    }

    // trylock example
    switch (mtx_trylock(&mu)) {
        case thrd_success: {
            sensitive_variable += 1;
            printf("Sensitive Variable: %d\n", sensitive_variable);

            mtx_unlock(&mu);
        } break;
        case thrd_busy: {
            printf("Can't trylock mutex: Mutex &mu is already locked");
        } break;
        default: {
            printf("Can't trylock mutex: Some error happened");
        } break;
    }

    // timedlock example
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);

    // Add 5s to timer
    ts.tv_sec += 5;
    switch (mtx_timedlock(&mu, &ts)) {
        case thrd_success: {
            sensitive_variable += sensitive_variable;
            printf("Sensitive Variable: %d\n", sensitive_variable);

            mtx_unlock(&mu);
        } break;
        case thrd_timedout: {
            printf("Can't timedlock mutex: Timeout reach");
        } break;
        default: {
            printf("Can't timedlock mutex: Some error happened");
        } break;
    }

    if (mtx_lock(&mu) == thrd_success) {
        sensitive_variable = (sensitive_variable - 1) / 2;
        printf("Sensitive Variable: %d\n", sensitive_variable);

        mtx_unlock(&mu);
    }

    mtx_destroy(&mu);
    return 0;
}
