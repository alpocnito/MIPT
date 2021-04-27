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

#define MAX(a, b) (a) > (b) ? (a) : (b)


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
  unsigned* cores;
  unsigned* real_cpu;
  unsigned* virtual_cpu;
  unsigned  real_cpu_num;
  unsigned  virtual_cpu_num;
};

const unsigned MaxCpu = 256;
#define FILE_AVAILABLE_CPU "/sys/devices/system/cpu/online"
#define FILE_TOPOLOGY_LENGTH (sizeof("/sys/devices/system/cpu/cpu0/topology/core_id") + 3)

const unsigned Alignment = 4096;
const unsigned Start = 0;
const unsigned Stop  = 1;

const long unsigned TotalSteps = 10000000000;

inline double function(double x)
{
  return x;
}

///////////////////////////////////////////////////////////////////////////
//!
//! Reads sys/devices/system/cpu/online and 
//!       sys/devices/system/cpu/cpu(%d)/topology/core_id
//!       and fills cpu_info struct
//!   
///////////////////////////////////////////////////////////////////////////
int GetCpuInfo(cpu_info_t* cpu_info, unsigned requested_threads = 1);

///////////////////////////////////////////////////////////////////////////
//!
//! Free all memory in cpu_info
//!
///////////////////////////////////////////////////////////////////////////
void FreeCpuInfo(cpu_info_t* cpu_info);

///////////////////////////////////////////////////////////////////////////
//!
//! Controls all n_threads threads
//!
///////////////////////////////////////////////////////////////////////////
int DistributeThreads(cpu_info_t* cpu_info, unsigned n_threads, double* ans);


