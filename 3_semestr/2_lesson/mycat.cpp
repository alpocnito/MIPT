#include <stdio.h>
#include <unistd.h>
#include <errno.h> 
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <color.h>

const size_t READ_NUM = 4096;

///////////////////////////////////////////////////////////////
//
// Write exactly count number of bytes, Otherwise, return -1
//  
// RETURN: -1 - in case of some error
//          0 - all is good
//
///////////////////////////////////////////////////////////////
ssize_t my_write(int fd, const char*  buf, size_t count);

///////////////////////////////////////////////////////////////
//
// Read ALL file, Otherwise, return -1
//  
// RETURN: -1         - in case of some error
//          succ_read - number read bytes
//
///////////////////////////////////////////////////////////////
ssize_t read_file(int fd, char** buf, size_t* cur_buf_size);

///////////////////////////////////////////////////////////////
//
// Read up to count bytes, Otherwise, return -1
//  
// RETURN: -1         - in case of some error
//          succ_read - number read bytes
//
///////////////////////////////////////////////////////////////
ssize_t my_read(int fd, char* buf, size_t count);

///////////////////////////////////////////////////////////////
//
// Open file, Otherwise, return -1
//  
// RETURN: -1 - in case of some error
//         fd - files descriptor
//
///////////////////////////////////////////////////////////////
int my_open(const char* path, int flags);

///////////////////////////////////////////////////////////////
//
// Close file, Otherwise, return -1
//  
// RETURN: -1 - in case of some error
//         fd - files descriptor
//
///////////////////////////////////////////////////////////////
int my_close(int fd);

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
  int in_fd  = 0;
  int out_fd = 1;

  char temp_str[READ_NUM];
  
  char*  file_buf     = NULL;
  size_t cur_buf_size = 0;
      
  ssize_t read_symbols = 0;
  if (argc == 1) 
  {
    while (1)
    { 
      if ((read_symbols = my_read(in_fd, temp_str, READ_NUM)) < 0)
        goto ERROR;
      if (my_write(out_fd, temp_str, (size_t)read_symbols) < 0)
        goto ERROR;
    }
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

      if(my_write(out_fd, file_buf, (size_t)read_symbols) < 0)
        goto ERROR;
    }
  }

  free(file_buf);
  return 0;

ERROR:
  free(file_buf);
  PRINT_MAGENTA(BOLD("Sorry, some error occurs\n"));
  return -1;
}
