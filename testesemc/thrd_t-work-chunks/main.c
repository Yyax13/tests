#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#define TASKS_CAP (1 << 24)
#define WORKERS_CAP 24

struct task_t {
    uint64_t (*func)(void *arg);
    void *arg;
};

struct worker_t {
    int id;
    size_t start;
    size_t end;

    uint64_t result;
    struct task_t *tasks;
};

uint64_t pseudoTask(void *arg) {
    int argument = *(int *)arg;
    uint64_t sink = 0xdeadbeef;

    for (int i = argument; i < 10000; i++) {
        argument ^= (i << 7);
        sink ^= argument;
    }

    return sink;
}

int worker_f(void *arg) {
    struct worker_t *self = (struct worker_t *)arg;
    for (size_t i = self->start; i < self->end; i++) {
        self->result ^= self->tasks[i].func(self->tasks[i].arg);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <workers> <tasks>\n", argv[0]);
        return 1;
    }

    int workersCount = strtol(argv[1], NULL, 10);
    if (workersCount <= 0 || workersCount > (int)WORKERS_CAP) {
        printf("Invalid <workers> value: must be between 1 and 16, received "
               "\"%s\"\n",
               argv[1]);
        return 1;
    }

    int tasksCount = strtol(argv[2], NULL, 10);
    if (tasksCount <= 0 || tasksCount > (int)TASKS_CAP) {
        printf("Invalid <tasks> value: must be between 1 and %d, received "
               "\"%s\"\n",
               (int)TASKS_CAP, argv[2]);
        return 1;
    }

    thrd_t threads[workersCount];
    struct worker_t workers[workersCount];
    struct task_t *tasks = malloc(tasksCount * sizeof(struct task_t));
    if (tasks == NULL)
        perror("Can't malloc tasks");

    int *taskArgs = malloc(tasksCount * sizeof(int));
    if (taskArgs == NULL)
        perror("Can't malloc taskArgs");

    for (int i = 0; i < tasksCount; i++) {
        taskArgs[i] = i % 64;
        tasks[i] = (struct task_t){.func = pseudoTask, .arg = &taskArgs[i]};
    }

    for (size_t i = 0; i < (size_t)workersCount; i++) {
        size_t chunk = tasksCount / workersCount;
        size_t remainder = tasksCount % workersCount;

        size_t start = i * chunk + (i < remainder ? i : remainder);
        size_t end = start + chunk + (i < remainder ? 1 : 0);
        workers[i] = (struct worker_t){.id = i,
                                       .start = start,
                                       .end = end,
                                       .tasks = tasks,
                                       .result = 0x05390539};

        thrd_create(&threads[i], worker_f, &workers[i]);
    }

    for (int i = 0; i < workersCount; i++)
        thrd_join(threads[i], NULL);

    printf("Program ended\n");
}
