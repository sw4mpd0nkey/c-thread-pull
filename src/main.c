#include <stdio.h>
#include <stdlib.h>

#include "../inc/threadpool.h"

int main() {

    printf("running malloc for new thread pool");
    threadpool_t *pool = malloc(sizeof(threadpool_t));
    printf("sending pool to threadpool_init");
    threadpool_init(pool);
}