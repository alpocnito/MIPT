#include <stdio.h>
#include <unistd.h>
#include <color.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


const char* Msg = "Hello, world!!!\n";
const size_t Msg_size = strlen(Msg);


#define SERVER_CAN_READ 1
#define CHILD_CAN_READ 0
const int NUMBER_SEM = 3;
const int MAX_MSG_SIZE = 1024;

#define TRY(cmd) \
    if ((cmd) < 0) { \
      perror(#cmd); \
      exit(EXIT_FAILURE); \
    }

#define Change(SEM_NAME, VAL) do {\
    op = {SEM_NAME, VAL, 0};  \
    TRY( semop(semid, &op, 1) ); \
    } while(0); \

void server(int semid, int in_pipe[2], int pipe1[2], int pipe2[2])
{ 
  sembuf op{};
  Change(SERVER_CAN_READ, -1);
  
  char msg[MAX_MSG_SIZE];
  long int read_num = read(in_pipe[0], msg, MAX_MSG_SIZE);
  TRY( read_num );

  TRY( write(pipe1[1], msg, (size_t)read_num) );
  TRY( write(pipe2[1], msg, (size_t)read_num) );

  Change(CHILD_CAN_READ, 2);
}

void client1(int semid, int pipefd[2], const char* msg, size_t msg_size)
{
  sembuf op{};
  
  TRY( write(pipefd[1], msg, msg_size) );
  Change(SERVER_CAN_READ, 1);
}

void client2and3(int semid, int pipefd[2])
{
  sembuf op{};
  
  Change(CHILD_CAN_READ, -1);
  
  char msg[MAX_MSG_SIZE];
  TRY( read(pipefd[0], msg, MAX_MSG_SIZE) );
  
  printf("Client get: %s\n", msg);
}

int main()
{
  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);
   
  int semid = semget(IPC_PRIVATE, NUMBER_SEM, 0777); 
  TRY( semid ); 

  int pipefd[3][2];
  for (int i = 0; i < 3; ++i)
    TRY( pipe(pipefd[i]) );
 
  // server
  pid_t pid = fork();
  TRY( pid );
  if (pid == 0)
  {
    server(semid, pipefd[0], pipefd[1], pipefd[2]);
    return 0;
  }
  
  // client1
  pid = fork();
  TRY( pid );
  if (pid == 0)
  {
    client1(semid, pipefd[0], Msg, Msg_size);
    return 0;
  }
 
  // client2and3
  for (int i = 0; i < 2; ++i)
  {
    pid = fork();
    TRY( pid );
    if (pid == 0)
    {
      client2and3(semid, pipefd[i+1]);
      return 0;
    }
  }
  return 0;
}
