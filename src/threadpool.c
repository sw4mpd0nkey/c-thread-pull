#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS 8
#define QUEUE_SIZE 100

void threadpool_init(threadpool_t* pool) {
    pool->queued = 0;
    pool->queued_front = 0;
    pool->queued_back = 0;
    pool->stop = 0;

    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify), NULL);

    for (int i = 0; i < THREADS; i++) {
       pthread_create(&(pool->threads[i]), NULL, thread_function, pool); 
    }
}

void threadpool_destroy(threadpool_t* pool) {

}

void threadpool_add_task(threadpool_t* pool, void (*function)(void*), void* arg) {

}

void example_task(void* arg) {

}