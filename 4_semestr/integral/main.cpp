#include "calc.h"

int main (int argc, char** argv) 
{
  if (argc != 2)
  {
    printf("Please, follow the format: ./calc <num>. There <num> - number of cores\n");
    return -1;
  }
  unsigned n_threads = unsigned(atoi(argv[1]));

  cpu_info_t cpu_info = {};
  GetCpuInfo(&cpu_info, n_threads);
  
#ifdef DEBUG
  printf("Real cpu number: %u, Virtual cpu number: %u\n", cpu_info.real_cpu_num, cpu_info.virtual_cpu_num);
  
  for (unsigned i = 0; i < cpu_info.real_cpu_num; ++i)
    printf("Real: %u\n", cpu_info.real_cpu[i]);
  
  for (unsigned i = 0; i < cpu_info.virtual_cpu_num; ++i)
    printf("Virtual: %u\n", cpu_info.virtual_cpu[i]);
  printf("---------------------------------------------------\n\n");
#endif

  double ans = 0;
  DistributeThreads(&cpu_info, n_threads, &ans);
  printf("Result: %lg\n", ans);

  FreeCpuInfo(&cpu_info);
  return 0;
}
