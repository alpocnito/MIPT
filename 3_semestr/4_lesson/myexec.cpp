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

bool ME_quiet = false;

int  AnalyseArguments(int argc, char **argv);
void PrintWC         (int pipefd);


int AnalyseArguments(int argc, char **argv)
{
  assert(argv);
  assert(*argv);

  // first parameter
  optind = 3;
  int next_argument = 0;
  int option_index = 0;
  static struct option long_options[] =
        {
          {"quiet",     no_argument, 0, 'q'},
          {0, 0, 0, 0}
        };

  while (1)
  {
    next_argument = getopt_long(argc, argv, "+q",
                    long_options, &option_index);
    // Detect the end of the options
    if (next_argument == -1)
      break;

    switch (next_argument)
    {
      case 'q':
        ME_quiet = true;
        break;
      
      case '?':
        break; 

      default:
        PRINT_RED_E(BOLD("Unknown argument: %c\n"), next_argument);
        return -1;
    }
  }

  return 0;
}

void PrintWC(int pipefd)
{
  char buf;
  bool maybe_word = 0;
  
  size_t words = 0;
  size_t bytes = 0;
  size_t lines = 0;
  
  while (read(pipefd, &buf, 1) > 0)
  {
    // for first words
    if (isalpha(buf) && words == 0)
      words = 1;
    
    // for words
    if (maybe_word && !isspace(buf))
    {
      words++;
      maybe_word = 0;
    }
    if (isspace(buf))
      maybe_word = 1;
    
    // print OR not print
    if (!ME_quiet)
      putc(buf, stdout);
  
    // lines
    if (buf == '\n')
      lines++;
    
    bytes++;
  }

  printf("%7lu %7lu %7lu\n", lines, words, bytes);
}



int main(int argc, char** argv)
{
  if (argc < 2)
    return 0;
  AnalyseArguments(argc, argv);

  int pipefd[2];
  if (pipe(pipefd) == -1)
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if (fork() == 0)
  {
    close(pipefd[0]);
    
    // stdou to pipe
    dup2(pipefd[1], 1);
    execvp(argv[2], argv + 2);
    
    perror("execvp error");
    exit(EXIT_FAILURE);
  }
  wait(NULL);
  
  close(pipefd[1]);
  PrintWC(pipefd[0]);
  close(pipefd[0]);
  
  return 0;
}

