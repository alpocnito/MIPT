#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
  if (argc < 2)
    return 0;
  
  timespec  tp{};
  
  clock_gettime(CLOCK_MONOTONIC, &tp);
  long int old_nanosecs = tp.tv_nsec;
  long int old_seconds  = tp.tv_sec;

  if (fork() == 0)
  {
    execvp(argv[1], argv + 1);
    perror("execvp error");
    exit(0);
  }
  wait(NULL);

  clock_gettime(CLOCK_MONOTONIC, &tp);
  long int new_nanosecs = tp.tv_nsec;
  long int new_seconds  = tp.tv_sec; 

  printf("%lg\n", (new_seconds - old_seconds) * 1000 + double(new_nanosecs - old_nanosecs) / 1000000);
  return 0;
}

