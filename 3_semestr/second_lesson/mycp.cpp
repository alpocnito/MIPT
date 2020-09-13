#include <stdio.h>
#include <unistd.h>
#include <errno.h> 
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <color.h>
#include <getopt.h>

const size_t READ_NUM = 4096;
const mode_t ALL_CAN_READ_WRITE_ACCESS = 0666;

bool CP_GiveVerbose = false;
bool CP_Interactive = false;
bool CP_Preserve    = false;
bool CP_Force       = false;

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
// Write exactly count number of bytes, Otherwise, return -1
//  
// RETURN: -1 - in case of some error
//          0 - all is good
//
///////////////////////////////////////////////////////////////
ssize_t my_write(int fd, const char*  buf, size_t count);

///////////////////////////////////////////////////////////////
//
// Delete file and open it
//  
// RETURN: -1 - in case of some error
//         fd - files descriptor
//
///////////////////////////////////////////////////////////////
int force_open(const char* path, int flags);

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

///////////////////////////////////////////////////////////////
//
// Read ALL file, Otherwise, return -1
//  
// RETURN: -1         - in case of some error
//          succ_read - number read bytes
//
///////////////////////////////////////////////////////////////
int AnalyseArguments(int argc, char **argv);



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

int force_open(const char* path, int flags = O_RDONLY)
{
  assert(path);

  // delete file
  if (unlink(path) < 0)
  {
    PRINT_RED_E(BOLD("Error: path=%s\n"), path);
    perror("force_open failed");         
    return -1;  
  }
  
  // create file
  return my_open(path, flags | O_CREAT);
}

int my_open(const char* path, int flags = O_RDONLY)
{
  assert(path);

  int fd = 0;
  while (1)
  {
    fd = open(path, flags, ALL_CAN_READ_WRITE_ACCESS); 
    if (fd < 0)                         //
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
  assert(fd >= 0);
  
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

int AnalyseArguments(int argc, char **argv)
{
  // first parameter
  optind = 3;
  int next_argument = 0;
  int option_index = 0;
  static struct option long_options[] =
        {
          {"verbose",     no_argument, 0, 'v'},
          {"force",       no_argument, 0, 'f'},
          {"interactive", no_argument, 0, 'i'},
          {"p",           no_argument, 0, 'p'},
          {0, 0, 0, 0}
        };

  while (1)
  {
    next_argument = getopt_long(argc, argv, "vfip",
                    long_options, &option_index);

    // Detect the end of the options
    if (next_argument == -1)
      break;

    switch (next_argument)
    {
      case 'v':
        CP_GiveVerbose = true;
        break;

      case 'i':
        CP_Interactive = true;
        break;

      case 'p':
        CP_Preserve = true;
        break;

      case 'f':
        CP_Force = true;
        break;
      
      case '?':
        PRINT_RED(BOLD("Error\n"));
        return -1;

      default:
        PRINT_RED_E(BOLD("Unknown argument: %c\n"), next_argument);
        return -1;
    }
  }

  return 0;
}


int main(int argc, char** argv)
{
  if (argc < 3)
  {
    PRINT_MAGENTA(BOLD("Too few arguments. Expected: mycp <src> <dest> [parameters]\n"));
    return -1;
  }
  
  // Arguments
  int in_flags  = O_RDONLY;
  int out_flags = O_WRONLY | O_CREAT;
  if (argc > 3)
    AnalyseArguments(argc, argv);

  int in_fd  = 0;
  int out_fd = 0;
  
  ssize_t read_symbols = 0;
  char*   buf          = NULL;
  size_t  buf_size     = 0;
  
  if (CP_Interactive && (access(argv[2], F_OK) != -1))        // CP_Interactive
  {
    printf("mycp: overwrite '%s'? ", argv[2]);
    char ans = 'n';
    scanf("%c", &ans);

    if (ans == 'n')
      return 0;
  }

  // open files
  if ((in_fd = my_open(argv[1], in_flags)) < 0)
    goto ERROR;

  if ((out_fd = my_open(argv[2], out_flags)) < 0)
    if (CP_Force)                                             // CP_Force
      if ((out_fd = force_open(argv[2], out_flags)) < 0)
        goto ERROR;
  
  // read and write
  if ((read_symbols = read_file(in_fd, &buf, &buf_size)) < 0)
    goto ERROR;
  if (my_write(out_fd, buf, (size_t)read_symbols) < 0)
    goto ERROR;
  
  my_close(in_fd);
  my_close(out_fd);
  free(buf);

  if (CP_GiveVerbose)                                          // CP_GiveVerbose
    printf("\'%s\' -> \'%s\'\n", argv[1], argv[2]);

  return 0;

ERROR:
  my_close(in_fd);
  my_close(out_fd);
  free(buf);
  return -1;
}
