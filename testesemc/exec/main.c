#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char* parseExecOutput(int pipeInt) {
    size_t bufferCap = 65536; // 64KB in init, can be higher
    size_t bufferLen = 0;
    char *buffer = malloc(bufferCap);
    if (buffer == NULL) {
        return NULL;

    }

    char _tmp[4096];
    ssize_t _n;

    while ((_n = read(pipeInt, _tmp, sizeof(_tmp))) > 0) {
        if ((bufferLen + _n) > bufferCap) {
            bufferCap *= 2;
            char *newBuffer = realloc(buffer, bufferCap);
            if (newBuffer == NULL) {
                free(buffer);
                return NULL;

            }

            buffer = newBuffer;

        }

        memcpy(buffer + bufferLen, _tmp, _n);
        bufferLen += _n;

    }

    buffer[bufferLen] = '\0';
    return buffer;

};


int _beacon_commands_exec(char *command) {
    printf("Starting exec\n");
    int pipeOut[2];
    pid_t procPID;

    pipe(pipeOut);

    procPID = fork();
    if (procPID < 0) {
        printf("Error: can't fork");
        return 0;

    } else if (procPID == 0) { // The following block looks complex and it's complex hahaha, i take 3 hours to do that s**t D:<
        printf("Starting in subproc");
        char *cmd = "/bin/sh";
        char cmd2[8192];
        snprintf(cmd2, sizeof(cmd2), "%s", command);
        char *cmdArgs[] = {"/bin/sh", "-c", cmd2};

        // Redirect stdout/err
        dup2(pipeOut[1], STDOUT_FILENO);
        dup2(pipeOut[1], STDERR_FILENO);

        close(pipeOut[0]);
        close(pipeOut[1]);

        execvp(cmd, cmdArgs);
        printf("error");
        _exit(127); // Exit 127 means "Command not found", the _exit instead of exit will not use atexit() handlers, GPT said that is the convention for subprocess that runs commands

    } else {
        printf("Starting in main\n");
        close(pipeOut[1]);

        char *execOutput = parseExecOutput(pipeOut[0]);
        close(pipeOut[0]);

        waitpid(procPID, NULL, 0);

        if (execOutput) {
            printf("%s", execOutput);
            free(execOutput);
            
        }
    }

    return 1;

};

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("\nUsage: %s \"<commands_here>\"\n\n", argv[0]);
        return 1;

    }

    _beacon_commands_exec(argv[1]);
    return 0;

}
