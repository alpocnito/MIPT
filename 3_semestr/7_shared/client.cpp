#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <color.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

const int SHM_SIZE   = 90000;
const int NUMBER_SEM = 2;
const size_t READ_NUM = 4096;
#define WAIT_SERVER 0
#define WAIT_CLIENT 1

#define Change(SEM_NAME, VAL) do {\
        op = {SEM_NAME, VAL, 0};  \
        TRY( semop(semid, &op, 1) ); \
    } while(0); \


#define TRY(cmd) \
    if ((cmd) < 0) { \
      perror(#cmd); \
      exit(EXIT_FAILURE); \
    }

ssize_t my_write(char* shared_memory, int fd, const char* buf, size_t count)
{
  assert(shared_memory);
  assert(buf);
  assert(fd >= 0);
  
  strncpy(shared_memory, buf, count);
  shared_memory[count] = '\0';

  return 0;
}

ssize_t read_file(int fd, char** buf, size_t* cur_buf_size)
{
  assert(cur_buf_size); assert(buf);
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
    if (status < 1)                 //
    {                               // 
      if (errno == EINTR)           // 
        continue;                   //
      if (status == 0 && errno == 0)// Analysing the read errors
        break;                      //
                                    //      
      PRINT_RED_E(BOLD("Error: path=%d\n"), fd);
      perror("my_read failed");     //
      return -1;                    //
    }                               //
    
    succ_read += (size_t)status;
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

int main(int argc, char** argv)
{
  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);
  key_t key = ftok("server", 1);
  TRY(key);

  int shmid = shmget(key, SHM_SIZE, IPC_CREAT);
  TRY(shmid)
  
  char* shared_memory = (char*)shmat(shmid, NULL, 0);
  if (shared_memory == NULL)
  {
    perror("shared_memory");
    exit(EXIT_FAILURE);
  }

  int semid = semget(key, NUMBER_SEM, 0777 | IPC_CREAT);
  TRY(semid);
  sembuf op{};

  PRINT_CYAN(BOLD("Client is waiting for server\n"));  
  Change(WAIT_SERVER, -1);
  PRINT_CYAN(BOLD("Client is waiting for working\n"));  
  Change(WAIT_CLIENT, -1);
  ////////////////////////////////////CRITICAL//////////////////////////////////
  PRINT_ORANGE(BOLD("Client is working\n"));  
  
  int in_fd  = 0;
  int out_fd = 1;

  char temp_str[READ_NUM];
  
  char*  file_buf     = NULL;
  size_t cur_buf_size = 0;
      
  ssize_t read_symbols = 0;
  if (argc == 1) 
  {
    if ((read_symbols = my_read(in_fd, temp_str, READ_NUM)) < 0)
      goto ERROR;
    if (my_write(shared_memory, out_fd, temp_str, (size_t)read_symbols) < 0)
      goto ERROR;
  }
  else
  {
    for (size_t i = 1; i < (size_t)argc; ++i)
    {
      int cur_fd = my_open(argv[i], O_RDONLY);
      if (cur_fd < 0)
        goto ERROR;

      if ((read_symbols = read_file(cur_fd, &file_buf, &cur_buf_size)) < 0)
        goto ERROR;
      
      if (my_close(cur_fd) < -1)
        goto ERROR;

      if(my_write(shared_memory, out_fd, file_buf, (size_t)read_symbols) < 0)
        goto ERROR;
    }
  }

  free(file_buf);

  ////////////////////////////////////CRITICAL//////////////////////////////////    
  Change(WAIT_CLIENT, 1);
  return 0;

ERROR:
  free(file_buf);
  PRINT_MAGENTA(BOLD("Sorry, some error occurs\n"));
  Change(WAIT_CLIENT, 1);
  return -1;
}
