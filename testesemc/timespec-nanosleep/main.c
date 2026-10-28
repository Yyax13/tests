#include <stdio.h>
#include <time.h>

struct timespec timespec_diff(struct timespec *start, struct timespec *end) {
    struct timespec diff = {.tv_sec = end->tv_sec - start->tv_sec,
                            .tv_nsec = end->tv_nsec - start->tv_nsec};

    if (diff.tv_nsec < 0) {
        diff.tv_sec -= 1;
        diff.tv_nsec += 1e9L;
    }

    return diff;
}

int main() {
    printf("== Nanosleep Demonstration\n");

    struct timespec start, end, diff;
    struct timespec rem, sleep = {.tv_sec = 3, .tv_nsec = 5e8};

    timespec_get(&start, TIME_MONOTONIC);
    int res = nanosleep(&sleep, &rem);
    if (res == -1) {
        perror("Retrying sleep");
        if ((res = nanosleep(&rem, NULL)) == -1) {
            perror("Can't sleep");
            return 1;
        }
    }

    timespec_get(&end, TIME_MONOTONIC);
    diff = timespec_diff(&start, &end);
    printf("==== Target sleep time: %ld.%09ld seconds\n", sleep.tv_sec,
           sleep.tv_nsec);
    printf("==== Slept for %ld.%09ld seconds\n", diff.tv_sec, diff.tv_nsec);

    return 0;
}
