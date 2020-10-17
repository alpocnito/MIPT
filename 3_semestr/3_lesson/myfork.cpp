#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void ForkLine(int number_childs)
{
  for (int i = 0; i < number_childs; ++i)
  {
    if (fork() == 0)
    {
      printf("child: pid:%d ppid:%d\n", getpid(), getppid());
      return;
    }
  }

  int status = 0;
  for (int i = 0; i < number_childs; ++i)
    printf("Waiting of %d completed\n", wait(&status));
}

void ForkDown(int number_childs)
{
  for (int i = 0; i < number_childs; ++i)
  {
    if (fork() != 0)
    {
      int status = 0;
      wait(&status);
      return;
    }
    printf("child: pid:%d ppid:%d\n", getpid(), getppid());
    continue;
  }

}


int main(int argc, char** argv)
{
  if (argc != 3)
    return 0;
  
  printf("parent: pid:%d\n", getpid());
  int number_childs = atoi(argv[2]);

  if (argv[1][0] == 'l')
    ForkLine(number_childs);
  else
    ForkDown(number_childs);


  return 0;
}

