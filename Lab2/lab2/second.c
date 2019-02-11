#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

pthread_mutex_t mutex;
int a = 0;

void *func (void *p) {
    pthread_mutex_lock(&mutex);
    a += 2;
    pthread_mutex_unlock(&mutex);
}

int main () {
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&thread1, NULL, func, NULL);
    pthread_create(&thread2, NULL, func, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_mutex_destroy(&mutex);
    printf("%d\n", a);
    return 0;
}
