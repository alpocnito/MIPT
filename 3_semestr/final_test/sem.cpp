#include <stdio.h>
#include <unistd.h>
#include <color.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/shm.h>

#define BOOK_PLACE 0
#define CAR_CAN_GO 1
#define BOAT_CAN_GO 2
#define SHARED_VAR 3
#define START 4

const int NUMBER_SEM = 5;

#define TRY(cmd) \
    if ((cmd) < 0) { \
      perror(#cmd); \
      exit(EXIT_FAILURE); \
    }

#define UP(SEM_NAME) do {\
  op = {SEM_NAME, 1, 0};\
  TRY( semop(semid, &op, 1) );\
  } while(0); \

#define DOWN(SEM_NAME) do {\
  op = {SEM_NAME, -1, 0};\
  TRY( semop(semid, &op, 1) );\
  } while(0); \

const size_t MAX_WAIT_BOATS = 4;

struct Shm {
  size_t cur_wait_boats = 0;
  size_t cur_wait_cars = 0;
};


void boat(int semid, int pass_name, Shm* shm)
{
  sembuf op{};
  DOWN(START);

  DOWN(SHARED_VAR);
  shm->cur_wait_boats++;
  UP(SHARED_VAR);

  PRINT_CYAN_E(N("Boat %d in the queue\n"), pass_name);
  DOWN(BOAT_CAN_GO);

  // CRITICAL
  DOWN(BOOK_PLACE)
  PRINT_CYAN_E(N("Boat %d on the bridge\n"), pass_name);
  
  DOWN(SHARED_VAR);
  shm->cur_wait_boats--;
  //printf("boats: %d, cars: %d\n", shm->cur_wait_boats, shm->cur_wait_cars);

  if (shm->cur_wait_boats >= MAX_WAIT_BOATS || (shm->cur_wait_cars == 0 && shm->cur_wait_boats != 0))
  {
    UP(BOAT_CAN_GO);
  }
  else
  {
    UP(CAR_CAN_GO);
  }
  UP(SHARED_VAR);

  PRINT_CYAN_E(N("Boat %d passed the bridge\n"), pass_name);
  UP(BOOK_PLACE);
  //CRITICAL
}
void car(int semid, int pass_name, Shm* shm)
{
  sembuf op{};
  DOWN(START);

  DOWN(SHARED_VAR);
  shm->cur_wait_cars++;
  UP(SHARED_VAR);

  PRINT_GREEN_E(N("Car %d in the queue\n"), pass_name);
  DOWN(CAR_CAN_GO);
  
  // CRITICAL
  DOWN(BOOK_PLACE)
  PRINT_GREEN_E(N("Car %d on the bridge\n"), pass_name);
  
  DOWN(SHARED_VAR);
  shm->cur_wait_cars--;

  //printf("boats: %d, cars: %d\n", shm->cur_wait_boats, shm->cur_wait_cars);

  if (shm->cur_wait_boats >= MAX_WAIT_BOATS || (shm->cur_wait_cars == 0 && shm->cur_wait_boats != 0))
  {
    UP(BOAT_CAN_GO);
  }
  else
  {
    UP(CAR_CAN_GO);
  }
  UP(SHARED_VAR);

  PRINT_GREEN_E(N("Car %d passed the bridge\n"), pass_name);
  UP(BOOK_PLACE);
  // CRITICAL
}

int main(int argc, char** argv)
{
  
  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);
  
  if (argc != 3)
  {
    printf("Invalid number of parameters\nDo:./sem <num_boats> <num_cars>\n");
    exit(-1);
  }
  
  int number_boats = atoi(argv[1]);
  int number_cars  = atoi(argv[2]);
  if (number_boats < 0 || number_cars < 0)
  {
    printf("Invalid input. Numbers must be positive\n");
    exit(-1);
  }
  int semid = semget(IPC_PRIVATE, NUMBER_SEM, 0777); 
  TRY( semid ); 
  sembuf op{};

  UP(SHARED_VAR);
  UP(BOOK_PLACE);
  
  // Open bridge
  if (number_boats < number_cars)
    UP(CAR_CAN_GO)
  else
    UP(BOAT_CAN_GO)

  // shared vars
  int shmid = shmget(IPC_PRIVATE, sizeof(Shm), IPC_CREAT | 0777);
  TRY(shmid);

  Shm* shm = (Shm*)calloc(1, sizeof(Shm));
  shm = (Shm*)shmat(shmid, NULL, 0);
  if (shm == NULL || (void*)shm == (void*)-1)
  {
    perror("shmat");
    exit(-1);
  }
  shm->cur_wait_boats = 0;
  shm->cur_wait_cars = 0;

  // make childs
  for (int i = 0; i < number_boats; ++i)
  {
    pid_t pid = fork();
    TRY( pid );

    if (pid == 0)
    {
      boat(semid, i + 1, shm);
      return 0;
    }
  }
  
  for (int i = 0; i < number_cars; ++i)
  {
    pid_t pid = fork();
    TRY( pid );

    if (pid == 0)
    {
      car(semid, i + 1, shm);
      return 0;
    }
  }

  // for normal start
  op = {START, (short int)(number_boats + number_cars), 0};
  TRY( semop(semid, &op, 1) );
  
  return 0;
}
