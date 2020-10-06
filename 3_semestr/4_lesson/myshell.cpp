#include "color.h"
#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <ctype.h>
#include <vector>

int main(int argc, char** argv)
{
  if (argc < 2)
    return 0;
  
  std::vector<size_t> pipes;
  pipes.push_back(0);
  for (size_t i = 0; i < (size_t)argc; ++i)
    if (argv[i][0] == ',')
    {
      printf("RE\n");
      pipes.push_back(i + 1);
      argv[i] = NULL;
    }
  
  // pipe for communication of parent and childs
  int pipefd[2];
  if (pipe(pipefd) == -1)
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  dup2(pipefd[0], 0);
  for (size_t i = 0; i < pipes.size(); ++i)
  {
    if (fork() == 0)
    {
      printf("CHILD GO: %d of %d\n", i, pipes.size());

     // if (i != pipes.size() - 1)
      //  dup2(pipefd[1], 1);
      
      if (execvp(argv[i], argv + i) == -1)
      {
        perror("execvp");
        exit(EXIT_FAILURE);
      }
    }
    wait(NULL);
  }

  return 0;
}

