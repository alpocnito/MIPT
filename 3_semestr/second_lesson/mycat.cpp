#include <stdio.h>
#include <unistd.h>

const size_t READ_NUM = 100000;

int main(int argc, char** argv)
{
  int in_fd = 0;
  int out_fd = 1;

  if (argc == 1) 
  {
    while (temp_str[0] != EOF)
    {  
      if (read(in_fd, temp_str, 1) < 0)
        perror("read problems");
      if (write(out_fd, temp_str, 1) < 0)
        perror("write problems");
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
  return 0;
}
