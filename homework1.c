#include "homework1.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <pthread.h>
#include <math.h>

int num_threads;
int resolution;
image *img;

void initialize(image *im) {
    im->width = resolution;
    im->max_value = 255;
    int i;
    im->matrix = calloc(im->width, sizeof(unsigned char*));
        for (i = 0; i < im->width; i++) {
            im->matrix[i] = calloc(im->width, sizeof(unsigned char));
        }
}

void* threadFunction(void *var)
{
    int i, j;
	int thread_id = *(int*)var;
	int start = thread_id * ceil((double)resolution/num_threads), end;
	if (((thread_id + 1) * ceil((double)resolution/num_threads)) < resolution) {
		end = (thread_id + 1) * ceil((double)resolution/num_threads);
	} else {
		end = resolution;
	}

    float pixel_dimension = (float)100/resolution;
    float x, y, distance;
    for(i = start; i < end; i++) {
        for (j = 0; j < resolution; j++) {
            x = i * (pixel_dimension) + pixel_dimension/2;
            y = j * (pixel_dimension) + pixel_dimension/2;
            distance = (abs(-1*x+2*y))/sqrt(5);
            if (distance < 3) {
                img->matrix[resolution - 1 - j][i] = 0;
            } else {
                img->matrix[resolution - 1 - j][i] = 255;
            }
        }
    }
    return NULL;
}

void render(image *im) {
    img = im;
    pthread_t tid[num_threads];
    int i;
    int thread_id[num_threads];
    for(i = 0; i < num_threads; i++) {
        thread_id[i] = i;
    }
    for(i = 0; i < num_threads; i++) {
        pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
    }
    for(i = 0; i < num_threads; i++) {
		pthread_join(tid[i], NULL);
	}
    
    // float pixel_dimension = (float)100/resolution;
    // float x, y, distance;
    // for(i = 0; i < resolution; i++) {
    //     for (j = 0; j < resolution; j++) {
    //         x = i * (pixel_dimension) + pixel_dimension/2;
    //         y = j * (pixel_dimension) + pixel_dimension/2;
    //         distance = (abs(-1*x+2*y))/sqrt(5);
    //         if (distance < 3) {
    //             im->matrix[resolution - 1 - j][i] = 0;
    //         } else {
    //             im->matrix[resolution - 1 - j][i] = 255;
    //         }
    //     }
    // }
}

void writeData(const char * fileName, image *img) {
    FILE *fp;
    fp = fopen(fileName, "wb");
        if (fp == NULL) {
        perror("The file can't be opened!");
    }

    int i, j;
    fprintf(fp, "P5\n%d %d\n%d\n", img->width, img->width, img->max_value);
    for (i = 0; i < img->width; i++) {
        for (j = 0; j < img->width; j++) {
            fwrite(&img->matrix[i][j], sizeof(unsigned char), 1, fp);
        }
    }
    fclose(fp)
;}

