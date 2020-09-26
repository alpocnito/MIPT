#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  if (argc < 3)
    return 0;

  for (int i = 0; i < argc - 2; ++i)
  {

    if (fork() == 0)
    {
      int cur_num = atoi(argv[i + 2]);

      usleep(10000 * (unsigned int)cur_num);
      printf("%d\n", cur_num);
      
      return 0;
    }
  }
  
  int status = 0;
  for (int i = 0; i < argc - 2; ++i)
    wait(&status);

  return 0;
}

