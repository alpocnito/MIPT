#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <color.h>

#define TRY(cmd) \
    do { if ((cmd) < 0) { \
      PRINT_RED_E(BOLD("Error on: %d"), __LINE__);\
      PRINT_WHITE(N("\n"));\
      perror(#cmd); \
      exit(EXIT_FAILURE); \
    } }while(0);


size_t Analyse(const char* dir_name)
{
  DIR* dir = opendir(dir_name);
  if (dir == 0)
  {
    if (errno == ENOTDIR)
    {
      struct stat statbuf;
      TRY(stat(dir_name, &statbuf));
      
      return statbuf
    }
    else
    {
      perror("opendir");
      exit(-1);
    }
  }
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

  //Analyse(argv[1]);

  return 0;
}
