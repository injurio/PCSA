#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#define NUM_THREADS 1000

pthread_t workers[NUM_THREADS];
struct {
    int count;
} shared_data;

void *run(void *arg)
{
    shared_data.count++;
    printf("Thread #%ld checked in\n", (long)arg);
    return NULL;
}

int main(void)
{
    int error;
    for (int i=0;i<NUM_THREADS;i++)
    {
        error = pthread_create(&(workers[i]), NULL, &run, (void *)i);
        if (error != 0)
            printf("\nThread can't be created : [%s]", strerror(error));
    }
    // make sure all threads are finished
    for (int i=0;i<NUM_THREADS;i++) {
        pthread_join(workers[i], NULL);
    }
    printf("shared_data.count = %d\n", shared_data.count);
    return 0;
}