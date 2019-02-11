#include "homework.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <pthread.h>
#include <math.h>

int num_threads;
int resize_factor;
image *in_global;
image *out_global;

void deallocate_dynamic_matrix(unsigned char **matrix, int row)
{
    int i;
    for (i = 0; i < row; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

void* threadFunction(void *var) {
    int imp = 16;
    int gaussian_kernel[3][3] = {  
        {1, 2, 1} ,   /*  initializers for row indexed by 0 */
        {2, 4, 2} ,   /*  initializers for row indexed by 1 */
        {1, 2, 1}     /*  initializers for row indexed by 2 */
    };
    if (resize_factor != 3) {
        imp = resize_factor * resize_factor;
    }
    long r=0, g=0, b=0;
    int i, j, k, l;
    int N = (*in_global).height;
    int P = num_threads;
    int thread_id = *(int*)var;
    int local_kernel = 1;
    int start = thread_id * ceil((double)N/P), end;
	if (((thread_id + 1) * ceil((double)N/P)) < N) {
		end = (thread_id + 1) * ceil((double)N/P);
	} else {
		end = N;
	}
    start -= (start%resize_factor);
    end -=(end%resize_factor);
        for (i = start; i < end; i+=resize_factor) {
            for (j = 0; j < (*in_global).width; j+=resize_factor) {
                for (k = 0; k < resize_factor; k++) {
                    for (l = 0; l < resize_factor; l++) {
                        if (resize_factor == 3) {
                            local_kernel = gaussian_kernel[k][l];
                        }
                        if ((*in_global).color_property == true) {
                            r += (*in_global).r[k+i][l+j] * local_kernel;
                            g += (*in_global).g[k+i][l+j] * local_kernel;
                            b += (*in_global).b[k+i][l+j] * local_kernel; 
                        } else {
                            r += (*in_global).matrix[k+i][l+j] * local_kernel;
                        }
                    }
                }
            if ((*out_global).color_property == true) {
                (*out_global).r[i/resize_factor][j/resize_factor] = (unsigned char)(r/imp);
                (*out_global).g[i/resize_factor][j/resize_factor] = (unsigned char)(g/imp);
                (*out_global).b[i/resize_factor][j/resize_factor] = (unsigned char)(b/imp);
            } else {
                (*out_global).matrix[i/resize_factor][j/resize_factor] = (unsigned char)(r/imp);
            }
            r = 0; g = 0; b = 0;
        }
    }
    return NULL;
}


void readInput(const char * fileName, image *img) {
    char* first_line = (char *)malloc(sizeof(char));
    char* width = (char *)malloc(sizeof(char));
    char* height = (char *)malloc(sizeof(char));
    char* maxval = (char *)malloc(sizeof(char));
    FILE* fp;
    fp = fopen (fileName, "rb");
    if (fp == NULL) {
        perror("The file can't be opened!");
    }
    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "%s\n%s %s\n%s\n",first_line, width, height, maxval);
    int row = atoi(height);
    int column = atoi(width);
    
    img->width = column;
    img->height = row;
    img->maxval = atoi(maxval);

    int size = column;
    int i = 0, j;
    int color = 1;
    if (strcmp("P6", first_line) == 0) {
        img->color_property = true;
        size *= 3;
        color *= 3;
        img->r = calloc(img->height, sizeof(unsigned char*));
        for (i = 0; i < img->height; i++) {
            img->r[i] = calloc(img->width, sizeof(unsigned char));
        }
        img->g = calloc(img->height, sizeof(unsigned char*));
        for (i = 0; i < img->height; i++) {
            img->g[i] = calloc(img->width, sizeof(unsigned char));
        }
        img->b = calloc(img->height, sizeof(unsigned char*));
        for (i = 0; i < img->height; i++) {
            img->b[i] = calloc(img->width, sizeof(unsigned char));
        }
    } else {
        img->color_property = false;
        img->matrix = calloc(img->height, sizeof(unsigned char*));
        for (i = 0; i < img->height; i++) {
            img->matrix[i] = calloc(img->width, sizeof(unsigned char));
        }
    }
    unsigned char buffer[size];
    for (i = 0; i < row; i++) {
        fread(buffer, sizeof(unsigned char) * color, column, fp);
        for (j = 0; j < column; j++) {
            if (img->color_property == false) {
                img->matrix[i][j] = buffer[j];
            } else {
                    img->r[i][j] = buffer[j * color];
                    img->g[i][j] = buffer[j * color + 1];
                    img->b[i][j] = buffer[j * color + 2];
                }
            }
    }
    fclose(fp);
}

void writeData(const char * fileName, image *img) {
    FILE *fp;
    int i, j;
    fp = fopen(fileName, "wb");
    if (fp == NULL) {
        perror("The file can't be opened!");
    }
    if (img->color_property == true) {
        fprintf(fp, "P6\n%d %d\n%d\n", img->width, img->height, img->maxval);
    } else {
        fprintf(fp, "P5\n%d %d\n%d\n", img->width, img->height, img->maxval);
    }
    if (img->color_property == false) {
        // for (i = 0; i < img->height; i++) {
        //     for (j = 0; j < img->width; j++) {
        //         fwrite(&img->matrix[i][j], sizeof(unsigned char), 1, fp);
        //     }
        // }
        for(i = 0; i < img->height; i++) {
            fwrite(img->matrix[i], sizeof(unsigned char), img->width, fp);
        }
    } else {
        // int k = 0, m = 0, n = 0;
        // for (i = 0; i < img->height; i++) {
        //     k = 0, m = 0, n = 0;
        //     for (j = 0; j < 3 * img->width; j++) {
        //         if (j%3 == 0) {
        //             fwrite(&img->r[i][k], sizeof(unsigned char), 1, fp);
        //             k += 1;
        //         } else if (j%3 == 1) {
        //             fwrite(&img->g[i][m], sizeof(unsigned char), 1, fp);
        //             m += 1;
        //         } else if (j%3 == 2) {
        //             fwrite(&img->b[i][n], sizeof(unsigned char), 1, fp);
        //             n += 1;
        //         }
        //     }
        // }
        for (i = 0; i < img->height; i++) {
            unsigned char row[img->width * 3];
            for (j = 0; j < img->width; j++) {
                row[j*3] = img->r[i][j];
                row[j*3+1] = img->g[i][j];
                row[j*3+2] = img->b[i][j];
            }
            fwrite(row, sizeof(unsigned char), img->width*3, fp);
        }
    }
    fclose(fp);
    if (img->color_property == true) {
        deallocate_dynamic_matrix(img->r, img->height);
        deallocate_dynamic_matrix(img->g, img->height);
        deallocate_dynamic_matrix(img->b, img->height);
    } else {
        deallocate_dynamic_matrix(img->matrix, img->height);
    }
}

void resize(image *in, image * out) {
    in->height = in->height - (in->height%resize_factor);
    in->width = in->width - (in->width%resize_factor);

    out->height = in->height/resize_factor;
    out->width = in->width/resize_factor;
    out->color_property = in->color_property;
    out->maxval = in->maxval;

    int i;
    if (in->color_property == false) {
    out->matrix = calloc(out->height, sizeof(unsigned char*));
    for (i = 0; i < out->height; i++) {
        out->matrix[i] = calloc(out->width, sizeof(unsigned char));
        }
    } else {
    out->r = calloc(out->height, sizeof(unsigned char*));
    for (i = 0; i < out->height; i++) {
        out->r[i] = calloc(out->width, sizeof(unsigned char));
    }
    out->g = calloc(out->height, sizeof(unsigned char*));
    for (i = 0; i < out->height; i++) {
        out->g[i] = calloc(out->width, sizeof(unsigned char));
    }
    out->b = calloc(out->height, sizeof(unsigned char*));
    for (i = 0; i < out->height; i++) {
        out->b[i] = calloc(out->width, sizeof(unsigned char));
        }
    }

    in_global = in;
	out_global = out;
    pthread_t tid[num_threads];
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
}