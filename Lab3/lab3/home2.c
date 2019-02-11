#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int a;
pthread_barrier_t barrier1;
pthread_barrier_t barrier2;

void* threadFunction1(void *var)
{
	pthread_barrier_wait(&barrier1);
	a = 5;
	pthread_barrier_wait(&barrier2);
	a += 7;
}

void* threadFunction2(void *var)
{
	a = 3;
	pthread_barrier_wait(&barrier1);
	pthread_barrier_wait(&barrier2);
	a += 2;
}

int main(int argc, char **argv)
{
	int P = 2;
	int i;

	pthread_t tid[P];
	int thread_id[P];
	for(i = 0;i < P; i++)
		thread_id[i] = i;

	pthread_barrier_init(&barrier1, NULL, P);
	pthread_barrier_init(&barrier2, NULL, P);

	pthread_create(&(tid[0]), NULL, threadFunction1, &(thread_id[0]));
	pthread_create(&(tid[1]), NULL, threadFunction2, &(thread_id[1]));

	for(i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	pthread_barrier_destroy(&barrier2);
	pthread_barrier_destroy(&barrier1);

	printf("HELLO THERE: %d\n", a);

	return 0;
}
