#include "macros.h"
#include <stdio.h>
#include <threads.h>

struct worker_args {
    int id;
    int begin;
    int end;
};

int worker(void *arg) {
    struct worker_args *args = arg;
    printf("worker %d: [%d, %d)\n", args->id, args->begin, args->end);

    volatile unsigned long x = 0;
    for (int i = args->begin; i < args->end; i++) {
        for (int _i = 0; _i < ITERATIONS; _i++) {
            x ^= i * _i;
        }
    }

    printf("worker %d: x -> %ld\n", args->id, x);
    return 0;
}

int main() {
    thrd_t threads[WORKERS_B];
    struct worker_args args[WORKERS_B];

    for (int i = 0; i < WORKERS_B; i++) {
        args[i] = (struct worker_args){.id = i,
                                       .begin = i * (TASKS / WORKERS_B),
                                       .end = (i + 1) * (TASKS / WORKERS_B)};

        thrd_create(&threads[i], worker, &args[i]);
    }

    for (int i = 0; i < WORKERS_B; i++)
        thrd_join(threads[i], NULL);
}
