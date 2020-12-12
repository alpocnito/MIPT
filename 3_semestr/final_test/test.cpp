#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <color.h>
#include <string>

#define TRY(cmd) \
    do { if ((cmd) < 0) { \
      PRINT_RED_E(BOLD("Error on: %d"), __LINE__);\
      PRINT_WHITE(N("\n"));\
      perror(#cmd); \
      exit(EXIT_FAILURE); \
    } }while(0);


long int Analyse(const char* dir_name)
{
  DIR* dir = opendir(dir_name);
  if (dir == 0)
  {
    if (errno == ENOTDIR)
    {
      struct stat statbuf;
      TRY(stat(dir_name, &statbuf));
      
      //printf("NAME: %s = %ld\n", dir_name, statbuf.st_blocks); 
      return statbuf.st_blocks;
    }
    else
    {
      perror("opendir");
      exit(-1);
    }
  }

  long int dir_size = 0;
  errno = 0;
  struct dirent* cur_object = NULL;
  while ((cur_object = readdir(dir)) != NULL)
  {
    if (std::string("..") == cur_object->d_name || std::string(".") == cur_object->d_name)
      continue;

    //printf("GET: %s\n", (std::string(dir_name) + "/" + cur_object->d_name).c_str());
    dir_size += Analyse((std::string(dir_name) + "/" + cur_object->d_name).c_str());
  }
  
  printf("%-7ld %s\n", dir_size, dir_name);

  return dir_size;
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    perror("Bad parameters\n");
    exit(-1);
  }
  
  struct stat statbuf;
  char* dir_name = argv[1];
  TRY(stat(dir_name, &statbuf));

  if ((statbuf.st_mode & S_IFMT) != S_IFDIR)
  {
    perror("This is not a dir_name\n");
    exit(-1);
  }

  Analyse(argv[1]);

  return 0;
}
