#include "calc.h"

#define Assert(expr) do {              \
                       if (!(expr)) {  \
                         printf("ERROR!\n"); \
                         printf("In %s:%d -> assert(", __PRETTY_FUNCTION__, __LINE__); \
                         puts(#expr); \
                         printf(")\n\n"); \
                       } } while (0); 

//#define DEBUG

///////////////////////////////////////////////////////////////////////////
//!
//! Add core_id, associated with virtual_core_number in cpu_info->real_cpu[]
//!
//! Return 1, if cire is added; 0 otherwise
///////////////////////////////////////////////////////////////////////////
int AddRealCore(cpu_info_t* cpu_info, unsigned virtual_core_number);

///////////////////////////////////////////////////////////////////////////
//!
//! Returns number of virtual threads in computer
//!
///////////////////////////////////////////////////////////////////////////
unsigned GetNumVirtThreads();

///////////////////////////////////////////////////////////////////////////
//!
//! Function, which each thread is do
//!
///////////////////////////////////////////////////////////////////////////
void* calculate(void* data);


int AddRealCore(cpu_info_t* cpu_info, unsigned virtual_core_number)
{
  assert(cpu_info);

  char file_topology_name[FILE_TOPOLOGY_LENGTH];
  sprintf(file_topology_name, "/sys/devices/system/cpu/cpu%u/topology/core_id", virtual_core_number);
  
  FILE* file_topology = fopen(file_topology_name, "r");
  Assert(file_topology);

  unsigned core_id = 0;
  Assert(fscanf(file_topology, "%u", &core_id) == 1);
  Assert(fclose(file_topology) == 0);

  
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
  
  //fclose(file_topology);
  return 0;
}


unsigned GetNumVirtThreads()
{
  unsigned max_number_threads = 0;
  
  // file with available virtual cpus
  FILE* file_available_cpus = fopen(FILE_AVAILABLE_CPU, "r");
  Assert(file_available_cpus);
 
  // parse file. It has format: a1-b1,a2-b2...
  unsigned a = 0, b = 0;
  while (fscanf(file_available_cpus, "%u-%u", &a, &b) == 2)
  {
    for (unsigned i = a; i <= b; ++i)
      if (b > max_number_threads)
        max_number_threads = b;
    
    // read comma
    char c;
    fscanf(file_available_cpus, "%c", &c);
  }
  
  fclose(file_available_cpus);
  return max_number_threads;
}


int GetCpuInfo(cpu_info_t* cpu_info, unsigned requested_threads)
{
  assert(cpu_info);
  
  unsigned max_number_threads = GetNumVirtThreads();
  if (requested_threads > max_number_threads)
    max_number_threads = requested_threads;
  
  cpu_info->cores =                  (unsigned*)calloc(max_number_threads, sizeof(unsigned));
  cpu_info->real_cpu =               (unsigned*)calloc(max_number_threads, sizeof(unsigned));
  cpu_info->virtual_cpu =            (unsigned*)calloc(max_number_threads, sizeof(unsigned));
  unsigned* repeated_virtual_cores = (unsigned*)calloc(max_number_threads, sizeof(unsigned));
  unsigned repeated_virtual_cores_num = 0;
 
  // file with available virtual cpus
  FILE* file_available_cpus = fopen(FILE_AVAILABLE_CPU, "r");
  Assert(file_available_cpus);

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
  Assert(fclose(file_available_cpus) == 0);
  
  for (unsigned i = 0; i < repeated_virtual_cores_num; ++i)
    cpu_info->virtual_cpu[(cpu_info->virtual_cpu_num)++] = repeated_virtual_cores[i];
  
  
  free(repeated_virtual_cores);
  return 0;
}

void FreeCpuInfo(cpu_info_t* cpu_info)
{
  assert(cpu_info);

  free(cpu_info->cores);
  free(cpu_info->real_cpu);
  free(cpu_info->virtual_cpu);
}


///////////////////////////////////////////////////////////////////////////
//!
//! Function, which each thread is do
//!
///////////////////////////////////////////////////////////////////////////
void* calculate(void* data) 
{
  assert(data);

  data_t* thread_data = (data_t*)data;
  
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(thread_data->cpu_n, &cpu_set);
  Assert(pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu_set) == 0);

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

int DistributeThreads(cpu_info_t* cpu_info, unsigned n_threads, double* ans)
{
  assert(cpu_info);
  
  // local array for results
  double** results = (double**)calloc(n_threads, sizeof(double*));
  Assert(results);

  pthread_t* pthread_id = (pthread_t*)calloc(MAX(cpu_info->virtual_cpu_num, n_threads), sizeof(pthread_t));
  Assert(pthread_id);

  struct data_t* thread_data = (data_t*)calloc(MAX(cpu_info->virtual_cpu_num, n_threads), sizeof(data_t));
  Assert(thread_data);
  
  //------choosing real cpus or virtual-------
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
  
  double big_step = (Stop - Start) / double(n_threads);
  unsigned cur_thread = 0;
  unsigned for_limit = MAX(cpu_info->virtual_cpu_num, n_threads);
  for (unsigned i = 0; i < for_limit; ++i) 
  {
    
    thread_data[i].start  = Start + double(0)     * big_step;
    thread_data[i].stop   = Start + double(0 + 1) * big_step;

    thread_data[i].step_n = TotalSteps / n_threads;
    thread_data[i].res    = (double*) memalign(Alignment, sizeof(double)); 
    thread_data[i].cpu_n  = cpu_info->virtual_cpu[i % cpu_info->virtual_cpu_num];
    Assert(thread_data[i].res);
   
    //-------------- Filling threads with normal work -------------
    if (n_threads < cpu_info->virtual_cpu_num)
    {
      for (unsigned j = 0; j < n_threads; ++j)
      {
        if (i == cpu_list[j % cpu_list_size])
        {
          results[cur_thread] = thread_data[i].res;    
          thread_data[i].start  = Start + double(cur_thread)     * big_step;
          thread_data[i].stop   = Start + double(cur_thread + 1) * big_step;
          cur_thread++;
        } 
      }
    }
    else
    {
      results[cur_thread++] = thread_data[i].res;
      thread_data[i].start  = Start + double(i)     * big_step;
      thread_data[i].stop   = Start + double(i + 1) * big_step; 
    }
    //-------------------------------------------------------------
   
    if (pthread_create(&pthread_id[i], NULL, &calculate, &thread_data[i]) != 0)
    {
      printf("Error!\n");
      printf("To much number of threads\n");
      FreeCpuInfo(cpu_info);
      return -1;
    }

#ifdef DEBUG
    printf("Created new thead with cpu: %u! Start from %f\n", cpu_info->virtual_cpu[i % cpu_info->virtual_cpu_num], thread_data[i].start);
#endif
  }

#ifdef DEBUG
  printf("Init has ended\n"); 
#endif
 
  double res = 0;
  for (size_t i = 0; i < n_threads; ++i) 
  {
    Assert(pthread_join(pthread_id[i], NULL) == 0);
#ifdef DEBUG
    printf("Thread with start = %f ended!\n", thread_data[i].start);
#endif
    res += *(results[i]);
  }
  
  *ans = res;
  return 0;
}

