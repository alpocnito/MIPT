#include <stdio.h>
#include <color.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <poll.h>

const size_t READ_NUM = 4096;
const int timeout_msecs = 2000;

#define TRY(cmd) \
    do { if ((cmd) < 0) { \
      PRINT_RED_E(BOLD("Error on: %d"), __LINE__);\
      PRINT_WHITE(N("\n"));\
      perror(#cmd); \
      exit(EXIT_FAILURE); \
    } }while(0);

int main()
{
  size_t num_childs = 10;
  
  int** pipes = (int**)calloc(num_childs, sizeof(pipes[0]));
  for (size_t i = 0; i < num_childs; ++i)
  {
    pipes[i] = (int*)calloc(2, sizeof(pipes[0][0]));
    TRY(pipe(pipes[i]));
  }
 
  struct pollfd* fds = (pollfd*)calloc(num_childs, sizeof(fds[0]));
  for (size_t i = 0; i < num_childs; ++i)
  {
    fds->fd = pipes[i][0];
    fds->events = POLLIN | POLLNVAL | POLLERR;
  }
  
  char word[] = "Hello!\n";
  TRY( write(pipes[0][1], word, sizeof(word)) );

//////////////////////////////// PARENT CYCLE //////////////////////////////
  printf("Go sycle\n");
  while (1)
  {
    int status = poll(fds, num_childs, timeout_msecs);
    if (status == 0)
      break;
    TRY(status);
    
    PRINT_GREEN(BOLD("HIT\n"));
  }
  printf("Stop cycle\n");  
  
  free(fds);
  for (size_t i = 0; i < num_childs; ++i)
    free(pipes[i]);
  free(pipes);

  return 0;
}
