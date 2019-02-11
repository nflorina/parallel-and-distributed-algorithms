#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))

int NReps;
int printLevel;
int N;
int* v;
int *vQSort;
int *vNew;
int P = 8;

pthread_barrier_t barrier;

void compareVectors(int * a, int * b) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		if(a[i]!=b[i]) {
			printf("Sorted incorrectly\n");
			return;
		}
	}
	printf("Sorted correctly\n");
}

void displayVector(int * v) {
	// DO NOT MODIFY
	int i;
	int displayWidth = 2 + log10(v[N-1]);
	for(i = 0; i < N; i++) {
		printf("%*i", displayWidth, v[i]);
		if(!((i+1) % 20))
			printf("\n");
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}

void getArgs(int argc, char **argv)
{
	if(argc < 4) {
		printf("Not enough paramters: ./program N NReps printLevel no_threads\nprintLevel: 0=no, 1=some, 2=verbouse\n");
		exit(1);
	}
	N = atoi(argv[1]);
	NReps = atoi(argv[2]);
	printLevel = atoi(argv[3]);
	//P = atoi(argv[4]);
}

void init()
{
	int i;
	v = malloc(sizeof(int) * N);
	vQSort = malloc(sizeof(int) * N);
	vNew = malloc(sizeof(int) * N);
	if(v == NULL) {
		printf("malloc failed!");
		exit(1);
	}

	// generate the vector v with random values
	// DO NOT MODIFY
	srand(42);
	for(i = 0; i < N; i++)
		v[i] = rand()%N;
}

void printPartial()
{
	int i;
	compareVectors(v, vQSort);
}

void printAll()
{
	displayVector(v);
	displayVector(vQSort);
	compareVectors(v, vQSort);
}

void print()
{
	if(printLevel == 0)
		return;
	else if(printLevel == 1)
		printPartial();
	else
		printAll();
}

void *threadFunction (void *var) {
	int thread_id = *(int *) var;

	int start = thread_id * ceil((double) N / (double) P);
	int end = min (N, ceil ((double) N / (double) P) * (thread_id + 1));

	int aux;

	for (int x = 0; x < N; ++x) {
		int final;
		if (end == N)
			final = end - 1;
		else
			final = end;
		for (int i = start; i < final; i += 2) {
			if(v[i] > v[i + 1]) {
				aux = v[i];
				v[i] = v[i + 1];
				v[i + 1] = aux;
			}
		}
		pthread_barrier_wait(&barrier);
		for (int i = start + 1; i < final; i += 2) {
			if(v[i] > v[i + 1]) {
				aux = v[i];
				v[i] = v[i + 1];
				v[i + 1] = aux;
			}
		}
		pthread_barrier_wait(&barrier);
	}

}

int main(int argc, char *argv[])
{
	int i, j, aux;
	getArgs(argc, argv);
	init();

	// make copy to check it against qsort
	// DO NOT MODIFY
	for(i = 0; i < N; i++)
		vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);

	pthread_t threads[P];
	int thread_id[P];
	pthread_barrier_init(&barrier, NULL, P);

	for (int i = 0; i < P; ++i) {
		thread_id[i] = i;
	}

	for (int i = 0; i < P; ++i) {
		pthread_create(&threads[i], NULL, threadFunction, &thread_id[i]);
	}

	// sort the vector v
	// PARALLELIZE ME
	/*
	int sorted = 0;
	while(!sorted) {
		sorted = 1;
		for(i = 0; i < N-1; i++) {
			if(v[i] > v[i + 1]) {
				aux = v[i];
				v[i] = v[i + 1];
				v[i + 1] = aux;
				sorted = 0;
			}
		}
	} */
	for (int i = 0; i < P; ++i) {
		pthread_join(threads[i], NULL);
	}
	pthread_barrier_destroy(&barrier);

	print();

	return 0;
}
