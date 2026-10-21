#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../inc/threadpool.h"

int main() {

    printf("running malloc for new thread pool");
    threadpool_t pool;
    printf("sending pool to threadpool_init");
    threadpool_init(&pool);

    // Add tasks to the thread pool
    for (int i = 0; i < 15; i++) {
        int* task_num = malloc(sizeof(int));
        *task_num = i;
        threadpool_add_task(&pool, example_task, task_num);
    }

    // Let tasks complete
    sleep(5);

    threadpool_destroy(&pool);

    return 0;
}