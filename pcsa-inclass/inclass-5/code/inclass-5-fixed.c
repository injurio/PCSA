#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#define NUM_THREADS 1000

pthread_t workers[NUM_THREADS];
// add volatile keyword
volatile struct {
    int count;
} shared_data;

// declare mutex
pthread_mutex_t mutex;

void *run(void *arg)
{
    // lock mutex
    pthread_mutex_lock(&mutex);
    shared_data.count++;
    // unlock mutex
    pthread_mutex_unlock(&mutex);
    printf("Thread #%ld checked in\n", (long)arg);
    return NULL;
}

int main(void)
{
    // initialize mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("\nmutex initialization has failed\n");
        return 1;
    }

    int error;
    for (int i=0;i<NUM_THREADS;i++) {
        error = pthread_create(&(workers[i]), NULL, &run, (void *)i);
        if (error != 0)
            printf("\nThread can't be created : [%s]", strerror(error));
    }

    // make sure all threads are finished
    for (int i=0;i<NUM_THREADS;i++) {
        pthread_join(workers[i], NULL);
    }

    printf("shared_data.count = %d\n", shared_data.count);

    // deallocate and destroy mutex
    pthread_mutex_destroy(&mutex);
    
    return 0;
}