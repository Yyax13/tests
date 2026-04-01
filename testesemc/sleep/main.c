#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

void whileSleep(long sleepTime) {
    long start = time(NULL);
    long end = start + sleepTime;
    while (time(NULL) < end) {
        struct timeval tv;
        long remainingSecs = end - time(NULL);

        if (remainingSecs <= 0) break;

        tv.tv_sec = (remainingSecs < 1) ? 0 : 1;
        tv.tv_usec = 0;

        select(0, NULL, NULL, NULL, &tv);
  
    }

};

int main() {
    printf("\nStarting with the PID %d\n", (int)getpid());
    printf("Starting at: %d\n", (int)time(NULL));

    whileSleep(199);

    printf("Ending at: %d\n", (int)time(NULL));

    return 0;

};
