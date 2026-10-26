#include "helper.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <time.h>

#ifndef WORKER_COUNT
#define WORKER_COUNT 8
#endif

struct worker_t {
    int id;
};

int worker_f(void *arg) {
    if (arg == NULL)
        return 1;

    struct worker_t self = *(struct worker_t *)(arg);

    struct timespec ts;
    timespec_get(&ts, TIME_THREAD_ACTIVE);

    printf("==== Thread %d: \t\t %ld.%09ld\n", self.id, ts.tv_sec, ts.tv_nsec);
    return 0;
}

int main(int argc, char *argv[]) {
    struct poc_mode_t mode = MODE_FULLY_FALSE;
    if (argc > 1) {
        if (argc > 2) {
            printf("Usage: %s <mode(s)>\n\n"
                   "mode:\n"
                   "\ttime \t - Current time\n"
                   "\tthread \t - Thread CPU Time\n"
                   "\tactive \t - Process CPU Time\n"
                   "\tmono \t - Monotonic timespec\n"
                   "\n"
                   "Example:\n"
                   "\t%s \"time|mono\""
                   "\n",
                   argv[0], argv[0]);

            return 1;
        }

        mode = getMode(getArgs(argv[1]));
    } else
        mode = MODE_FULLY_TRUE;

    printf("== timespec demonstration\n\n");
    struct timespec ts;

    if (mode.time) {
        timespec_get(&ts, TIME_UTC);

        printf("== Current time (with TIME_UTC):\n");
        printf("==== Seconds: \t\t %ld\n", ts.tv_sec);
        printf("==== Nanosseconds: \t %ld\n\n", ts.tv_nsec);
    }

    if (mode.threadActive) {
        printf("== Thread time (with TIME_THREAD_ACTIVE):\n");

        const size_t workersCount = WORKER_COUNT;
        struct worker_t workers[workersCount];
        thrd_t threads[workersCount];

        for (int i = 0; i < workersCount; i++) {
            workers[i] = (struct worker_t){.id = i + 1};
            if (thrd_create(&threads[i], worker_f, &workers[i]) !=
                thrd_success) {
                printf("Can't create thread %d\n", i);
                return 1;
            }
        }

        for (int i = 0; i < workersCount; i++) {
            if (thrd_join(threads[i], NULL) != thrd_success) {
                printf("Can't join the thread %d\n", i);
                return 1;
            }
        }
    }

    if (mode.active) {
        timespec_get(&ts, TIME_ACTIVE);

        printf("\n== Active time (with TIME_ACTIVE):\n");
        printf("==== Seconds: \t\t %ld\n", ts.tv_sec);
        printf("==== Nanosseconds: \t %ld\n\n", ts.tv_nsec);
    }

    if (mode.mono) {
        timespec_get(&ts, TIME_MONOTONIC);
        printf("== Monotonic time (with TIME_MONOTONIC):\n");
        printf("==== Monotonic A: \t %ld.%09ld\n", ts.tv_sec, ts.tv_nsec);

        struct timespec tsB;
        timespec_get(&tsB, TIME_MONOTONIC);
        printf("==== Monotonic B: \t %ld.%09ld\n", tsB.tv_sec, tsB.tv_nsec);

        struct timespec diff = timespec_diff(ts, tsB);
        printf("==== Difference: \t %ld.%09ld\n", diff.tv_sec, diff.tv_nsec);
    }

    return 0;
}
