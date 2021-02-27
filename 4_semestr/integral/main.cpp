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
#include <sched.h>
#include <malloc.h>

#define ALIGNMENT 4096
#define N_CORES 8
#define START 0
#define STOP  1
#define STEPS_PER_THREAD 300000000

inline float function(float x)
{
  return x;
}

typedef struct data_t data_t;
struct data_t 
{
    float start;
    float stop;
    float* res;
};

void* calculate(void* data) 
{
  float* res = (float*) calloc(1, sizeof(float));
  
  struct data_t* thread_data = (struct data_t*)data;
  float start = thread_data->start;
  float stop  = thread_data->stop;
  float step  = (stop - start) / STEPS_PER_THREAD;
  assert(step > 0);

  float temp_res = 0;
  for (size_t i = 0; i < STEPS_PER_THREAD; ++i) 
  {
    temp_res += step * ( function(start + float(i) * step) + function(start + float(i + 1) * step) ) / 2;
  }
  *(thread_data->res) = temp_res;
  //printf("Temp res: %f\n", temp_res);
  printf("Near to end. Start: %f\n", start);

  pthread_exit(NULL);
}

int main (int argc, char** argv) 
{
  if (argc != 2)
  {
    printf("Please, follow the format: ./calc <num>. There <num> - number of cores\n");
    return -1;
  }

  size_t n_threads = size_t(atoi(argv[1]));
  
  float start = START;
  float stop  = STOP;
  float big_step = (stop - start) / float(n_threads);

  float** results = (float**)calloc(n_threads, sizeof(float*));
  assert(results);

  pthread_t* pthread_id = (pthread_t*)calloc(n_threads, sizeof(pthread_t));
  assert(pthread_id);

  struct data_t* thread_data = (struct data_t*)calloc(n_threads, sizeof(struct data_t));
  assert(thread_data);

  for (size_t i = 0; i < n_threads; ++i) 
  {
    thread_data[i].start = start + float(i) * big_step;
    thread_data[i].stop  = start + float(i + 1) * big_step;
    thread_data[i].res   = (float*) memalign(ALIGNMENT, sizeof(float));
    
    assert(thread_data[i].res);
    results[i] = thread_data[i].res;
  
    //*(thread_data->res) = 1;
    //printf("%f\n", *(results[i]));
    
    assert(pthread_create(&pthread_id[i], NULL, &calculate, &thread_data[i]) == 0); 

    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    CPU_SET(i % N_CORES, &cpu_set);
    assert(pthread_setaffinity_np(pthread_id[i], sizeof(cpu_set_t), &cpu_set) == 0);
    printf("Setted affinity. Start: %f\n", thread_data[i].start);
  }
 
  float res = 0;
  for (size_t i = 0; i < n_threads; ++i) 
  {
    res += *(results[i]);
  }
 
  for (size_t i = 0; i < n_threads; ++i) 
  {
    assert(pthread_join(pthread_id[i], NULL) == 0);
    printf("Joined. Start: %f\n", thread_data[i].start);
  }

  printf("Result: %f\n", res);
  
  return 0;
}

