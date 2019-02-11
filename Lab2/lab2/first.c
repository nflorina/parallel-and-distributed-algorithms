#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


pthread_barrier_t barrier;

void *first (void *p) {
    printf("1\n");
    pthread_barrier_wait(&barrier);
}

void *second (void *p) {
    pthread_barrier_wait(&barrier);
    printf("2\n");
}

int main () {
    pthread_t thread[2];
    int id[2];
    pthread_barrier_init(&barrier, NULL, 2);
    for (int i = 0; i < 2; ++i) {
        id[i] = i;
    }
    pthread_create(&(thread[0]), NULL, first, &(id[0]));
    pthread_create(&(thread[1]), NULL, second, &(id[1]));

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);

    pthread_barrier_destroy(&barrier);

    return 0;
}