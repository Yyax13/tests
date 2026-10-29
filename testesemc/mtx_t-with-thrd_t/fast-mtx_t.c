#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <time.h>

#define TASKS_CAP (1 << 24)
#define WORKERS_CAP 24

struct task_t {
    uint64_t (*func)(void *arg);
};

uint64_t sum;
mtx_t mu;

struct worker_t {
    int id;
    size_t start;
    size_t end;

    struct task_t *tasks;
};

uint64_t pseudoTask(void *arg) {
    for (int i = 0; i < 10000; i++)
        (*(uint64_t *)arg)++;

    return 0;
}

int worker_f(void *arg) {
    struct worker_t *self = (struct worker_t *)arg;
    uint64_t accumulator = 0;
    for (size_t i = self->start; i < self->end; i++)
        self->tasks[i].func(&accumulator);

    mtx_lock(&mu);
    sum += accumulator;
    mtx_unlock(&mu);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <workers> <tasks>\n", argv[0]);
        return 1;
    }

    int workersCount = strtol(argv[1], NULL, 10);
    if (workersCount <= 0 || workersCount > (int)WORKERS_CAP) {
        printf("Invalid <workers> value: must be between 1 and %d, received "
               "\"%s\"\n",
               (int)WORKERS_CAP, argv[1]);
        return 1;
    }

    int tasksCount = strtol(argv[2], NULL, 10);
    if (tasksCount <= 0 || tasksCount > (int)TASKS_CAP) {
        printf("Invalid <tasks> value: must be between 1 and %d, received "
               "\"%s\"\n",
               (int)TASKS_CAP, argv[2]);
        return 1;
    }

    if (mtx_init(&mu, mtx_plain | mtx_recursive) != thrd_success) {
        printf("Can't init mtx_t mu\n");
        return 1;
    }

    thrd_t threads[workersCount];
    struct worker_t workers[workersCount];
    struct task_t *tasks = malloc(tasksCount * sizeof(struct task_t));
    if (tasks == NULL)
        perror("Can't malloc tasks");

    printf("Expected result: %lu\n", tasksCount * 10000UL);
    for (int i = 0; i < tasksCount; i++)
        tasks[i] = (struct task_t){.func = pseudoTask};

    for (size_t i = 0; i < (size_t)workersCount; i++) {
        size_t chunk = tasksCount / workersCount;
        size_t remainder = tasksCount % workersCount;

        size_t start = i * chunk + (i < remainder ? i : remainder);
        size_t end = start + chunk + (i < remainder ? 1 : 0);
        workers[i] = (struct worker_t){
            .id = i, .start = start, .end = end, .tasks = tasks};

        thrd_create(&threads[i], worker_f, &workers[i]);
    }

    for (int i = 0; i < workersCount; i++)
        thrd_join(threads[i], NULL);

    printf("Result: %lu\n", sum);
    printf("\nProgram ended\n");

    mtx_destroy(&mu);
    free(tasks);
    return 0;
}
