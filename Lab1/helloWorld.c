#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>


void* threadFunction(void *var)
{
	int thread_id = *(int*)var, i;
	for (i = 0; i < 20; ++i) {
		printf("Hello world from thread %i at iteration %d\n", thread_id, i);
	}
}

int main(int argc, char **argv)
{
	int P = 6;
	int i;
	long int no_threads = sysconf(_SC_NPROCESSORS_ONLN);

	pthread_t tid[no_threads];
	int thread_id[no_threads];
	pthread_mutex_t mutex; 
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_lock(&mutex);
	printf("Number of cores used: %ld\n", sysconf(_SC_NPROCESSORS_ONLN));
	for(i = 0;i < no_threads; i++)
		thread_id[i] = i;

	for(i = 0; i < no_threads; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}

	for(i = 0; i < no_threads; i++) {
		pthread_join(tid[i], NULL);
	}
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);	
	return 0;
}
