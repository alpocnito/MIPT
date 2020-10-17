#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  int ppid = (int)getppid();
  int pid  = (int)getpid();

  printf("pid:  %d\nppid: %d\n", pid, ppid);

  return 0;
}
