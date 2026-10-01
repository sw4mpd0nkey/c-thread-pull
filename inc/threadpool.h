#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

#define THREADS 16
#define QUEUE_SIZE 100

// fn: A pointer to the function that will be executed.
// arg: A pointer to the argument that will be passed to the function.
typedef struct {
    void (*fn)(void * arg)
    void* arg;
} task_t;

// structure manages the entire thread pool, including 
// the threads themselves, the task queue, and the synchronization mechanisms.
typedef struct {
    // A mutex used to synchronize access to the task queue.
    pthread_mutex_t lock;
    // A condition variable to notify threads when a new task is available
    pthread_cond_t notify;
    // An array of threads that make up the thread pool
    pthreead_t threads[THREAD_POOL_SIZE];
    // An array representing the queue of tasks waiting to be executed.
    task_t task_queue[QUEUE_SIZE];
    // The maximum size of the task queue.
    int queued;
    // The index of the front of the task queue.
    int queued_front;
    // The index of the rear of the task queue.
    int queued_back;
    // A flag to indicate when the thread pool should stop processing tasks and terminate.
    int stop;
} threadpool_t;


// Function declarations
void threadpool_init(threadpool_t* pool);
void threadpool_destroy(threadpool_t* pool);
void threadpool_add_task(threadpool_t* pool, void (*function)(void*), void* arg);
void example_task(void* arg);


#endif