#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define REPS 100

int a;

void* threadFunction(void *var)
{
	int thread_id = *(int*)var;

	for (int i = 0; i < REPS; ++ i)
		a += 2;

	printf("Hello world from thread %i with a = %d\n", thread_id, a);
}

int main(int argc, char **argv)
{
	int P = 2;
	int i;

	pthread_t tid[P];
	int thread_id[P];
	for(i = 0;i < P; i++)
		thread_id[i] = i;

	for(i = 0; i < P; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}

	for(i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	printf("A = %d\n", a);

	return 0;
}
