#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct data_t data_t;
struct data_t 
{
    float start;
    float step;
    int n_calc;
};

void* calculate(void* data) 
{
    float* res = (float*) calloc(1, sizeof(float));
    struct data_t* thread_data = (struct data_t*) data;
    float start = thread_data->start;
    float step = thread_data->step;
    float n_calc = thread_data->n_calc;

    float x = 0;

    for (int i = 0; i < n_calc; ++i) {
        x = start + i * step;
        *res += sqrtf(1 - x * x) * step;
        //fprintf(stderr, "Counting res in one of threads: %f\n", *res);
    }
    pthread_exit(res);
    //return (void*) res;
}

void func() {
    fprintf(stdout, "My turn!\n");
}

int main (int argc, char** argv) 
{
  if (argc != 2)
  {
    printf("Please, follow the format: ./calc <num>. There <num> - number of cores\n");
    return -1;
  }

  size_t number_cores = atoi(argv[1]);

    assert(argc == 3);
    int col = strtol(argv[1], &argv[1], 10);
    int n_threads = strtol(argv[2], &argv[2], 10);
    fprintf(stderr, "Cols: %d\nThreads number: %d\n", col, n_threads);

    float start = -1;
    float end = 1;
    float step = (end - start) / col;
    float n_calc = 1.0 * col / n_threads;
    float res = 0;

    float** results = (float**) calloc(n_threads, sizeof(float*));
    assert(results);


    pthread_t* pthread_id = (pthread_t*) calloc(n_threads, sizeof(pthread_t));
    assert(pthread_id);

    struct data_t* thread_data = (struct data_t*) calloc(n_threads, sizeof(struct data_t));
    struct timeval end_time;
    struct timeval start_time;

    gettimeofday(&start_time, NULL);

    for (int i = 0; i < n_threads; ++i) {
        thread_data[i].n_calc = n_calc;
        thread_data[i].start = start + i * n_calc * step;
        thread_data[i].step = step;
        fprintf(stderr, "Thread %d data: n_calc = %d, start = %f, step = %f\n", i,
                        thread_data[i].n_calc,
                        thread_data[i].start,
                        thread_data[i].step);
        pthread_create(&pthread_id[i], NULL, &calculate, &thread_data[i]);
        pthread_join(pthread_id[i], (void**) &results[i]);
    }

    for (int i = 0; i < n_threads; ++i) {
        fprintf(stdout, "Results[%d]: %f\n", i, *(results[i]));
        res += *(results[i]);
    }

    fprintf(stdout, "Result: %f\n", 2 * res);
    gettimeofday(&end_time, NULL);
    fprintf(stdout, "Time: %d\n", end_time.tv_usec - start_time.tv_usec);

    // for (int i = 0; i < col; ++i) {
    //     x_val[i] = x;
    //     fprintf(stderr, "x_val[%d] = %f\n", i, x);
    //     x = x + step;
    // }




    return 0;
}

