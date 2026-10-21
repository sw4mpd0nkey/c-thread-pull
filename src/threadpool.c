#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../inc/threadpool.h"

void* thread_function(void* threadpool) {

    if (threadpool == NULL) return NULL;

    threadpool_t* pool = (threadpool_t*)threadpool;

    while (1) {
        //lock the mutex on the pool itself
        pthread_mutex_lock(&(pool->lock));

        // waits for pool to notify and then unlocks pool
        while (pool->queue_size == 0 && !pool->stop) {
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }

        // make sure pool isnt stopped
        if (pool->stop) {
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }

        // 
        task_t task = pool->task_queue[pool->queue_front];
        pool->queue_front = (pool->queue_front + 1) % QUEUE_SIZE;
        pool->queued--;

        pthread_mutex_unlock(&(pool->lock));

        (*(task.fn))(task.arg);
    }

    return NULL;
}

void threadpool_init(threadpool_t* pool) {

    if (pool == NULL) return;

    pool->queue_size = QUEUE_SIZE;
    pool->queued = 0;
    pool->queue_front = 0;
    pool->queue_back = 0;
    pool->stop = 0;

    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify), NULL);

    for (int i = 0; i < THREADS; i++) {
       pthread_create(&(pool->threads[i]), NULL, thread_function, &pool); 
    }
}

void threadpool_destroy(threadpool_t* pool) {
    pool->queued = 0;
    pool->queue_front = 0;
    pool->queue_back = 0;
    pool->stop = 0;

    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
}

void threadpool_add_task(threadpool_t* pool, void (*function)(void*), void* arg) {

    pthread_mutex_lock(&(pool->lock));

    int next_back = (pool->queue_back + 1) % QUEUE_SIZE;
    if (pool -> queued < QUEUE_SIZE) {
        pool->task_queue[pool->queue_back].fn = function;
        pool->task_queue[pool->queue_back].arg = arg;
        pool->queue_back = next_back;
        pthread_cond_signal(&(pool->notify));
    } else {
        printf("Task queue is full, cant add more tasks\n");
    }

    pthread_mutex_unlock(&(pool->lock));
}

void example_task(void* arg) {

    int* num = (int*)arg;
    printf("Processing task %d\n", num);
    sleep(1);
    free(arg);
}