#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 4
#define MAX_ELEM 1000

int arr[MAX_ELEM];
int next = 0;
pthread_mutex_t mutex; // Declare a mutex

void *thread_update(void *arg) {
    int tid = *(int *)arg;

    while (1) {
        pthread_mutex_lock(&mutex); // Lock the mutex to protect shared data
        if (next >= MAX_ELEM) {
            pthread_mutex_unlock(&mutex); // Unlock the mutex
            break; 
        }
        printf("%d %d\n", next, tid); 
        arr[next++] = tid; 
        pthread_mutex_unlock(&mutex); // Unlock the mutex
    }
    free(arg);
    pthread_exit(NULL);
}


int main() {
    pthread_t threads[MAX_THREADS];
    pthread_mutex_init(&mutex, NULL); // Initialize the mutex

    for (int tid = 0; tid < MAX_THREADS; tid++) {
        int *arg = malloc(sizeof(int));
        *arg = tid;
        pthread_create(&threads[tid], NULL, thread_update, arg);
    }

    for (int tid = 0; tid < MAX_THREADS; tid++)
        pthread_join(threads[tid], NULL);

    pthread_mutex_destroy(&mutex); // Destroy the mutex

    // Print the elements in the arr array
    for (int i = 0; i < MAX_ELEM; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}
