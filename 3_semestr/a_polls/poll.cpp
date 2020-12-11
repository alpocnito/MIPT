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

ssize_t my_write(int fd, const char* buf, size_t count)
{
  assert(buf);
  assert(fd >= 0);

  size_t succ_write = 0;
  while (succ_write < count)
  {
    ssize_t status = write(fd, buf + succ_write, count - succ_write);

    if (status < 1)                //
    {                              //
      if (!errno || errno == EINTR)// 
        continue;                  //
                                   // Analysing the write errors
                                   //
      PRINT_RED_E(BOLD("Error: path=%d\n"), fd);
      perror("my_write failed");   //
      return -1;                   //
    }                              //
                                   
    succ_write += (size_t)status;
  }

  return 0;
}

ssize_t read_file(int fd, char** buf, size_t* cur_buf_size)
{
  assert(cur_buf_size); 
  assert(buf);
  assert(fd >= 0);

  size_t succ_read = 0;
  if (*cur_buf_size < READ_NUM)
  {
      *buf = (char*)realloc(*buf, READ_NUM);
      *cur_buf_size = READ_NUM;
      if (buf == NULL)
      {
        perror("problems with realloc");
        return -1;
      }
  }

  while (1)
  {
    // memory allocation
    if (succ_read != 0)
    {
      *buf = (char*)realloc(*buf, *cur_buf_size + READ_NUM);
      *cur_buf_size += READ_NUM;
      if (buf == NULL)
      {
        perror("problems with realloc");
        return -1;
      }
    }

    // reading
    ssize_t status = read(fd, *buf + succ_read, READ_NUM);
    while (errno == EINTR || errno == EAGAIN)
    {
      printf("OOOPS\n");
      status = read(fd, *buf + succ_read, READ_NUM);
    }
    TRY(status);
    succ_read += (size_t)status;
    if ((size_t)status < READ_NUM)
      break;
  }
    
  return (ssize_t)succ_read;
}

ssize_t my_read(int fd, char* buf, size_t count)
{
  assert(buf);
  assert(fd >= 0);
  size_t succ_read = 0;
  
  while (1)
  {
    ssize_t status = read(fd, buf, count);
    
    if (status < 1)                 //
    {                               //
      if (errno == EINTR)           // 
        continue;                   //
      if (status == 0 || errno == 0)// Analysing the read errors
        break;                      //
                                    //      
      PRINT_RED_E(BOLD("Error: path=%d\n"), fd);
      perror("my_read failed");     //
      return -1;                    //
    }                               //
    
    succ_read += (size_t)status;
    break;
  }
    
  return (ssize_t)succ_read;
}

int my_open(const char* path, int flags = O_RDONLY)
{
  assert(path);

  int fd = 0;
  while (1)
  {
    fd = open(path, flags); 
    if (fd < 1)                         //
    {                                   // 
      if (errno == EINTR)               // 
        continue;                       // Analysing the open errors        
                                        //      
      PRINT_RED_E(BOLD("Error: path=%s\n"), path);
      perror("my_open failed");         //
      return -1;                        //
    }                                   //
    break;
  }

  return fd;
}

int my_close(int fd)
{
  assert(fd > 0);
  
  int status = 0;
  while (1)
  {
    status = close(fd);
    
    if (status < 0)                 //
    {                               // 
      if (errno == EINTR)           // 
        continue;                   // Analysing the close errors        
                                    //      
      PRINT_RED_E(BOLD("Error: path=%d\n"), fd);
      perror("my_close failed");    //
      return -1;                    //
    }                               //
    break;
  }

  return fd;
}

int child_job(int child_pipe[2], int father_pipe[2])
{
  char*  buf = NULL;
  size_t buf_size = 0;
  
  printf("%d,%d: Wanna read\n", child_pipe[0], child_pipe[1]);
  ssize_t file_size = read_file(father_pipe[0], &buf, &buf_size);
  if (file_size <= 0)
    return -1;
  
  printf("%d,%d: Read: %s", child_pipe[0], child_pipe[1], buf);
  TRY(my_write(child_pipe[1], buf, (size_t)file_size))
  printf("%d,%d: Writed!\n\n", child_pipe[0], child_pipe[1]);
  
  TRY(my_close(child_pipe[0]));
  TRY(my_close(child_pipe[1]));
  TRY(my_close(father_pipe[0]));
  TRY(my_close(father_pipe[1]));
  free(buf);
  return 0;
}

void init_childs(int** childs_pipes, int** father_pipes, char** argv, size_t num_childs)
{
  for (size_t i = 0; i < num_childs; ++i)
  {
    PRINT_GREEN_E(N("New child on: %d,"), childs_pipes[i][0]);
    PRINT_GREEN_E(N("%d\n"), childs_pipes[i][1]);
    pid_t child = fork();
    TRY(child);

    if (child == 0)
    {
      if (i == 0)
      {
        int fd = my_open(argv[1], O_RDONLY);
        TRY(fd);
        TRY(my_close(father_pipes[i][0]));
        father_pipes[i][0] = fd;
      }
      if (i == num_childs - 1)
      {
        int fd = my_open(argv[2], O_WRONLY);
        TRY(fd);
        TRY(my_close(childs_pipes[i][1]));
        childs_pipes[i][1] = fd;
      }
      
      TRY(child_job(childs_pipes[i], father_pipes[i]));
      exit(0);
    }
  }
}

int main(int argc, char** argv)
{
  if (argc != 4 || atoi(argv[3]) < 2)
  {
    PRINT_RED(BOLD("Bad parameters\n"));
    return -1;
  }
  
  size_t num_childs = (size_t)atoi(argv[3]);

  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);

//////////////////////////////// LOGIC ////////////////////////////////////
  // Childs write to this pipe
  int** childs_pipes = (int**)calloc(num_childs, sizeof(childs_pipes[0]));
  for (size_t i = 0; i < num_childs; ++i)
  {
    childs_pipes[i] = (int*)calloc(2, sizeof(childs_pipes[0][0]));
    TRY(pipe(childs_pipes[i]));
  }
  // Father writes to this pipe
  int** father_pipes = (int**)calloc(num_childs, sizeof(father_pipes[0]));
  for (size_t i = 0; i < num_childs; ++i)
  {
    father_pipes[i] = (int*)calloc(2, sizeof(father_pipes[0][0]));
    TRY(pipe(father_pipes[i]));
  }

  init_childs(childs_pipes, father_pipes, argv, num_childs);
  
  struct pollfd* fds = (pollfd*)calloc(num_childs, sizeof(fds[0]));
  for (size_t i = 0; i < num_childs; ++i)
  {
    fds[i].fd = childs_pipes[i][0];
    fds[i].events = POLLIN | POLLNVAL | POLLERR;
  }

//////////////////////////////// PARENT CYCLE //////////////////////////////
  printf("Go sycle\n");
  while (1)
  {
    int status = poll(fds, num_childs, timeout_msecs);
    if (status == 0)
      break;
    TRY(status);
    
    for (size_t i = 0; i < num_childs; ++i)
    {
      if ( (fds[i].revents & POLLERR) || (fds[i].revents & POLLNVAL) )
        TRY(-1);

      if (fds[i].revents & POLLIN)
      {
        char* buf = NULL;
        size_t buf_size = 0;

        ssize_t file_size = read_file(childs_pipes[i][0], &buf, &buf_size);
        TRY(file_size);
        
        printf("%d,%d: GOT: %s", childs_pipes[i][0], childs_pipes[i][1], buf);
        
        TRY(my_write(father_pipes[i+1][1], buf, (size_t)file_size));
        printf("Writed in parent cycle\n\n");
        
        free(buf);
      }
    }
  }
  printf("Stop cycle\n");  
  
  free(fds);
  for (size_t i = 0; i < num_childs; ++i)
  {
    TRY(my_close(childs_pipes[i][0]));
    TRY(my_close(childs_pipes[i][1]));
    TRY(my_close(father_pipes[i][0]));
    TRY(my_close(father_pipes[i][1]));
    free(father_pipes[i]);
    free(childs_pipes[i]);
  }
  free(childs_pipes);
  free(father_pipes);

  return 0;
}
