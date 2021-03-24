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

//#define DEBUG

#define MAX_CPU 256
#define FILE_AVAILABLE_CPU "/sys/devices/system/cpu/online"
#define FILE_TOPOLOGY_LENGTH (sizeof("/sys/devices/system/cpu/cpu0/topology/core_id") + 3)

#define ALIGNMENT 4096
#define N_CORES 8
#define START 0
#define STOP  1
const long unsigned TOTAL_STEPS = 10000000000;

inline double function(double x)
{
  return x;
}

typedef struct data_t data_t;
struct data_t 
{
  double start;
  double stop;
  long unsigned step_n;
  double* res;
  unsigned cpu_n;
};

typedef struct cpu_info_t cpu_info_t;
struct cpu_info_t
{
  unsigned cores[MAX_CPU];
  unsigned real_cpu[MAX_CPU];
  unsigned virtual_cpu[MAX_CPU];
  unsigned real_cpu_num;
  unsigned virtual_cpu_num;
};

///////////////////////////////////////////////////////////////////////////
//!
//! Add core_id, associated with virtual_core_number in cpu_info->real_cpu[]
//!
//! Return 1, if cire is added; 0 otherwise
///////////////////////////////////////////////////////////////////////////
int AddRealCore(cpu_info_t* cpu_info, unsigned virtual_core_number)
{
  assert(cpu_info);

  char file_topology_name[FILE_TOPOLOGY_LENGTH];
  sprintf(file_topology_name, "/sys/devices/system/cpu/cpu%u/topology/core_id", virtual_core_number);
  
  FILE* file_topology = fopen(file_topology_name, "r");
  assert(file_topology);

  unsigned core_id = 0;
  assert(fscanf(file_topology, "%u", &core_id) == 1);
  assert(fclose(file_topology) == 0);

  
  // insert core_id in cpu_info
  int core_finded = 0;
  for (unsigned j = 0; j < cpu_info->real_cpu_num; ++j)
  {
    if (cpu_info->cores[j] == core_id)
    {
      core_finded = 1;
      break;
    }
  }
  if (!core_finded)
  {
    cpu_info->cores[cpu_info->real_cpu_num] = core_id;
    cpu_info->real_cpu[cpu_info->real_cpu_num] = virtual_core_number;
    cpu_info->real_cpu_num++;
    
    return 1;
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////
//!
//! Reads sys/devices/system/cpu/online and 
//!       sys/devices/system/cpu/cpu(%d)/topology/core_id
//!       and fills cpu_info struct
//!   
///////////////////////////////////////////////////////////////////////////
int GetCpuInfo(cpu_info_t* cpu_info)
{
  assert(cpu_info);

  cpu_info->virtual_cpu_num = 0;
  cpu_info->real_cpu_num = 0;

  // file with available virtual cpus
  FILE* file_available_cpus = fopen(FILE_AVAILABLE_CPU, "r");
  assert(file_available_cpus);
 
  unsigned repeated_virtual_cores[MAX_CPU];
  unsigned repeated_virtual_cores_num = 0;

  // parse file. It has format: a1-b1,a2-b2...
  unsigned a = 0, b = 0;
  while (fscanf(file_available_cpus, "%u-%u", &a, &b) == 2)
  {
    for (unsigned i = a; i <= b; ++i)
    {
      if (AddRealCore(cpu_info, i))
        cpu_info->virtual_cpu[(cpu_info->virtual_cpu_num)++] = i;
      else
        repeated_virtual_cores[repeated_virtual_cores_num++] = i;
    }
    
    // read comma
    char c;
    fscanf(file_available_cpus, "%c", &c);
  }
  assert(fclose(file_available_cpus) == 0);
  
  for (unsigned i = 0; i < repeated_virtual_cores_num; ++i)
    cpu_info->virtual_cpu[(cpu_info->virtual_cpu_num)++] = repeated_virtual_cores[i];

  return 0;
}

///////////////////////////////////////////////////////////////////////////
//!
//! Function, which each thread is do
//!
///////////////////////////////////////////////////////////////////////////
void* calculate(void* data) 
{
  data_t* thread_data = (data_t*)data;
  
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(thread_data->cpu_n, &cpu_set);
  assert(pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu_set) == 0);

  double start = thread_data->start;
  double stop  = thread_data->stop;
  double step  = (stop - start) / double(thread_data->step_n);
  assert(step > 0);
  

#ifdef DEBUG
  printf("CPU = %d for start: %0.3lg;\n", sched_getcpu(), start);
#endif

  double temp_res = 0;
  for (long unsigned i = 0; i < thread_data->step_n; ++i) 
  {
    temp_res += function(start + double(i) * step);
  }
  *(thread_data->res) = temp_res * step;

#ifdef DEBUG
  printf("Thread with start: %lg near to end\n", start);
#endif

  pthread_exit(NULL);
}

///////////////////////////////////////////////////////////////////////////
//!
//! Controls all n_threads threads
//!
///////////////////////////////////////////////////////////////////////////
int DistributeThreads(cpu_info_t* cpu_info, unsigned n_threads)
{
  assert(cpu_info);
  
  double start = START;
  double stop  = STOP;
  double big_step = (stop - start) / double(n_threads);

  // local array for results
  double** results = (double**)calloc(n_threads, sizeof(double*));
  assert(results);

  pthread_t* pthread_id = (pthread_t*)calloc(n_threads, sizeof(pthread_t));
  assert(pthread_id);

  struct data_t* thread_data = (data_t*)calloc(n_threads, sizeof(data_t));
  assert(thread_data);
  

  //------choosing real cpus or virtual-------
  unsigned  current_cpu = 0;
  unsigned* cpu_list;
  unsigned  cpu_list_size;
  if (n_threads > cpu_info->real_cpu_num)
  {
    cpu_list = cpu_info->virtual_cpu;
    cpu_list_size = cpu_info->virtual_cpu_num;
  }
  else
  {
    cpu_list = cpu_info->real_cpu;
    cpu_list_size = cpu_info->real_cpu_num;
  }
  //-------------------------------------------
  
  for (unsigned i = 0; i < n_threads; ++i) 
  {
    thread_data[i].start  = start + double(i) * big_step;
    thread_data[i].stop   = start + double(i + 1) * big_step;
    thread_data[i].step_n = TOTAL_STEPS / n_threads;
    thread_data[i].res    = (double*) memalign(ALIGNMENT, sizeof(double)); 
    thread_data[i].cpu_n  = cpu_list[current_cpu % cpu_list_size];
    current_cpu++;

    assert(thread_data[i].res);
    results[i] = thread_data[i].res;
    
    assert(pthread_create(&pthread_id[i], NULL, &calculate, &thread_data[i]) == 0); 

#ifdef DEBUG
    printf("Created new thead with cpu: %u! Start from %f\n", cpu_list[current_cpu % cpu_list_size], thread_data[i].start);
#endif
  }

#ifdef DEBUG
  printf("Init has ended\n"); 
#endif
 
  double res = 0;
  for (size_t i = 0; i < n_threads; ++i) 
  {
    assert(pthread_join(pthread_id[i], NULL) == 0);
#ifdef DEBUG
    printf("Thread with start = %f ended!\n", thread_data[i].start);
#endif
    res += *(results[i]);
  }
  
  printf("Result: %f\n", res);
  
  return 0;
}

int main (int argc, char** argv) 
{
  if (argc != 2)
  {
    printf("Please, follow the format: ./calc <num>. There <num> - number of cores\n");
    return -1;
  }
  unsigned n_threads = unsigned(atoi(argv[1]));

  cpu_info_t cpu_info = {};
  GetCpuInfo(&cpu_info);
  
#ifdef DEBUG
  printf("Real cpu number: %u, Virtual cpu number: %u\n", cpu_info.real_cpu_num, cpu_info.virtual_cpu_num);
  
  for (unsigned i = 0; i < cpu_info.real_cpu_num; ++i)
    printf("Real: %u\n", cpu_info.real_cpu[i]);
  
  for (unsigned i = 0; i < cpu_info.virtual_cpu_num; ++i)
    printf("Virtual: %u\n", cpu_info.virtual_cpu[i]);
  printf("---------------------------------------------------\n\n");
#endif
  
  DistributeThreads(&cpu_info, n_threads);
  
  return 0;
}

