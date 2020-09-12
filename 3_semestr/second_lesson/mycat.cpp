#include <stdio.h>
#include <unistd.h>
#include <errno.h> 
#include <assert.h>
#include <stdlib.h>

const size_t READ_NUM = 4096;

ssize_t my_write(int fd, const char* buf, size_t count);
ssize_t my_read (int fd,       char* buf, size_t count);


ssize_t my_write(int fd, const char* buf, size_t count)
{
  assert(buf);
  
  size_t succ_write = 0;
  while (succ_write < count)
  {
    ssize_t status = write(fd, buf + succ_write, count - succ_write);

    if (status < 1)                //
    {                              //
      if (!errno || errno == EINTR)// 
        continue;                  //
                                   // Analysing the write errors
      perror("my_write failed");   //
      return -1;                   //
    }                              //
                                   
    succ_write += (size_t)status;
  }

  return 0;
}


ssize_t my_read(int fd, char* buf, size_t count)
{
  size_t succ_read = 0;
  
  while (1)
  {
    ssize_t status = read(fd, buf, count);

    if (status < 1)                //
    {                              //
      if (!errno || errno == EINTR)// 
        continue;                  //
                                   // Analysing the read errors
      perror("my_read failed");    //
      return -1;                   //
    }                              //
    
    succ_read += (size_t)status;
    break;
  }
    
  return (ssize_t)succ_read;
}

int main(int argc, char** argv)
{
  int in_fd = 0;
  int out_fd = 1;

  char temp_str[READ_NUM];

  if (argc == 1) 
  {
    while (1)
    { 
      ssize_t read_symbols = 0;
      if ((read_symbols = my_read(in_fd, temp_str, READ_NUM)) < 0)
        goto ERROR;
      if (my_write(out_fd, temp_str, (size_t)read_symbols) < 0)
        goto ERROR;
    }
  }/*
  else
  {
    for (size_t i = 1; i < argc; ++i)
    {
      int cur_fd = myopen(argv[i], O_RDONLY);
      my_write(cur_fd, out_fd);
    }


  }*/

  free(temp_str);
  return 0;

ERROR:
  free(temp_str);
  printf("Sorry, some error occurs\n");
  return -1;
}
