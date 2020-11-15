#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <color.h>
#include <errno.h>
#include <stdlib.h>

const int SHM_SIZE   = 90000;
const int NUMBER_SEM = 2;
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

void server()
{
  key_t key = ftok("server", 1);
 
  int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0777);
  TRY(shmid)
  
  char* shared_memory = (char*)shmat(shmid, NULL, 0);
  if (shared_memory == NULL || (void*)shared_memory == (void*)-1)
  {
    perror("shared_memory");
    exit(EXIT_FAILURE);
  }

  int semid = semget(key, NUMBER_SEM, 0777 | IPC_CREAT);
  TRY(semid);
  sembuf op{};
  
  unsigned short initial[2] = {1, 1};
  semctl(semid, 0, SETALL, initial);

  while (1)
  {
    PRINT_CYAN(BOLD("Server is waiting\n"));
    Change(WAIT_SERVER, 0);
    Change(WAIT_CLIENT, -1);
    
    PRINT_ORANGE(BOLD("Server is writing\n"));
    if (shared_memory[0] != '\0')
    {
      printf("%s", shared_memory);
      shared_memory[0] = '\0';
    }
    Change(WAIT_CLIENT, 1);
    Change(WAIT_SERVER, 1);
    sleep(1);
  }
}

int main()
{
  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);
  server();   
  return 0;
}
