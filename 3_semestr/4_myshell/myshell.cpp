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
#include <string.h>

#define TRY(cmd) \
  if ((cmd) < 0) \
  {        \
    perror(#cmd); \
    exit(EXIT_FAILURE); \
  }
const size_t MAX_NUMBER_COMMANDS = 10;
const size_t MAX_NUMBER_KEYS     = 10;
const size_t LONGEST_WORD        = 20;

char* ReadCommand()
{ 
  char* str = NULL;
  size_t len = 0;
  
  TRY(getline(&str, &len, stdin));
  for (size_t i = 0; i < len; ++i)
    if (str[i] == '\n')
      str[i] = '\0';
  
  return str;
}


size_t DivideCommand(char* str, char*** commands)
{
  (*commands) = (char**)calloc(MAX_NUMBER_COMMANDS, sizeof(commands[0]));
  
  (*commands)[0] = str;
  size_t cur_pos = 1;
  char*  cur_ptr = strchr(str, ',');

  while (cur_ptr)
  {
    (*commands)[cur_pos++] = cur_ptr + 1;
    *cur_ptr = NULL;
    str = cur_ptr + 1;
    
    cur_ptr = strchr(str, ',');
  }
  
  return cur_pos;
}

void GivePrompt()
{
  PRINT_GREEN(BOLD("[ron]> "));
}

char** SplitToCommands(char* str)
{
  char** splitted_str = (char**) calloc(MAX_NUMBER_KEYS, sizeof(splitted_str[0]));
  char* pch = strtok(str, " ");

  size_t i = 0;
  while (pch != NULL)
  {
    splitted_str[i++] = (char*)calloc(LONGEST_WORD, sizeof(splitted_str[0][0]));
    strcpy(splitted_str[i-1], pch);
    
    pch = strtok(NULL, " ");
  }

  return splitted_str;
}

int main()
{
  GivePrompt();
  
  char** commands  = NULL;
  size_t ncommands = 0;
  
  char* str = ReadCommand();
  ncommands = DivideCommand(str, &commands);
  
  int next_in = 0;
  for (size_t i = 0; i < ncommands; ++i)
  {
    // pipe for communication of parent and childs
    int pipefd[2];
    TRY(pipe(pipefd));
    
    pid_t pchild = fork();
    TRY(pchild);
    
    // main if
    if (pchild == 0)
    {
      char** exec_args = NULL;
      exec_args = SplitToCommands(commands[i]);
      
      TRY( dup2(next_in, 0));
      
      // if it is NOT the last command
      if (i != ncommands - 1)
        TRY( dup2(pipefd[1], 1));
      
      TRY( close(pipefd[0]));
      TRY( execvp(exec_args[0], exec_args));
    }
    wait(NULL);
    next_in = pipefd[0];

    TRY(close(pipefd[1]));
  }

  free(commands);
  free(str);
  return 0;
}

