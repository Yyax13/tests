#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static struct timespec timespec_diff(struct timespec start,
                                     struct timespec end) {
    struct timespec diff = {.tv_sec = end.tv_sec - start.tv_sec,
                            .tv_nsec = end.tv_nsec - start.tv_nsec};

    if (diff.tv_nsec < 0) {
        diff.tv_sec--;
        diff.tv_nsec += 1000000000L;
    }

    return diff;
}

struct poc_mode_t {
    bool time;
    bool threadActive;
    bool active;
    bool mono;
};

#define MODE_FULLY_TRUE                                                        \
    (struct poc_mode_t) { true, true, true, true }
#define MODE_FULLY_FALSE                                                       \
    (struct poc_mode_t) { false, false, false, false }

struct _strArray {
    char **array;
    size_t len;
};

static struct _strArray *getArgs(char *arg) {
    // Nullity
    if (arg == NULL || arg[0] == 0x00) {
        printf("Can't parse arg: *arg == NULL\n");
        return NULL;
    }

    size_t argLen = strlen(arg);
    if (arg[0] == '|' || arg[argLen - 1] == '|') {
        printf("Invalid argument: The '|' separator must be between two "
               "arguments\n");
        return NULL;
    }

    struct _strArray *out =
        (struct _strArray *)malloc(sizeof(struct _strArray) * 1);

    if (out == NULL) {
        perror("Can't malloc out");
        return NULL;
    }

    // Multiple modes
    if (strchr(arg, '|') != NULL) {
        char *ptr = arg;

        size_t count = 0;
        while ((ptr = strchr(ptr, '|')) != NULL) {
            count++;
            ptr++;
        }

        out->len = count + 1;
        out->array = (char **)malloc(sizeof(uintptr_t) * out->len);
        if (out->array == NULL) {
            perror("Can't malloc out->array");
            free(out);
            return NULL;
        }

        for (int i = 0; i < out->len; i++) {
            size_t offset = strcspn(arg, "|");
            arg[offset] = 0x00;
            out->array[i] = arg;
            arg += offset + 1;
        }

        return out;
    } else {
        out->len = 1;
        out->array = (char **)malloc(sizeof(uintptr_t));
        if (out->array == NULL) {
            perror("Can't malloc out->array");
            free(out);
            return NULL;
        }

        out->array[0] = arg;
        return out;
    }
}

#define poc_mode_TABLE                                                         \
    X("time", out.time)                                                        \
    X("thread", out.threadActive)                                              \
    X("active", out.active)                                                    \
    X("mono", out.mono)

static struct poc_mode_t getMode(struct _strArray *strs) {
    if (strs == NULL)
        return MODE_FULLY_FALSE;

    struct poc_mode_t out = MODE_FULLY_FALSE;
    for (int i = 0; i < strs->len; i++) {
        char *arg = strs->array[i];

#define X(str, p)                                                              \
    if (strcmp(arg, str) == 0) {                                               \
        p = true;                                                              \
        continue;                                                              \
    }

        poc_mode_TABLE
#undef X

            printf("Invalid argument \"%s\": Skipping...\n", arg);
    }

    free(strs->array);
    free(strs);
    return out;
}
