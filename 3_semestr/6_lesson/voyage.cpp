#include <stdio.h>
#include <unistd.h>
#include <color.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>

#define WANT_GO_COAST 0
#define LADDER_DOWN 4
#define NUMBER_ON_BOAT 1
#define NUMBER_ON_COST 2
#define NUMBER_ON_LADDER 3
#define VOYAGE_ENDED 5
#define VOYAGE_STARTED 6
#define WAS_LAST_TRIP 7
const int NUMBER_SEM = 8;

#define TRY(cmd) \
    if ((cmd) < 0) { \
      perror(#cmd); \
      exit(EXIT_FAILURE); \
    }

#define Change(SEM_NAME, VAL) do {\
    op = {SEM_NAME, VAL, 0};  \
    TRY( semop(semid, &op, 1) ); \
    } while(0); \

void passanger(int semid, int pass_name)
{
  sembuf op{};
  while (1)
  {
    Change(LADDER_DOWN, 0);
    if (semctl(semid, WAS_LAST_TRIP, GETVAL))
      break;

    Change(NUMBER_ON_BOAT, -1);
    
    if (semctl(semid, WAS_LAST_TRIP, GETVAL))
      break;

    Change(NUMBER_ON_LADDER, -1);
    PRINT_GREEN_E(N("Passangers %d on ladder\n"), pass_name);
    Change(NUMBER_ON_LADDER, 1);
    PRINT_GREEN_E(N("Passangers %d on boat\n"), pass_name);
    
    Change(VOYAGE_STARTED, 0);
    Change(VOYAGE_ENDED, 0);

    Change(LADDER_DOWN, 0);
    Change(NUMBER_ON_LADDER, -1);
    PRINT_GREEN_E(N("Passangers %d on ladder\n"), pass_name);
    Change(NUMBER_ON_LADDER, 1);
    PRINT_GREEN_E(N("Passangers %d on coast\n"), pass_name);
    Change(NUMBER_ON_BOAT, 1);
    Change(WANT_GO_COAST, -1);
  }
  PRINT_GREEN_E(BOLD("Passangers %d thanks for trip\n"), pass_name);
}

void captain(int semid, short int boat_size, short int ladder_size, int num_steps)
{ 
  sembuf op{};
  Change(LADDER_DOWN, -1);
  Change(VOYAGE_STARTED, -1);
 
  for (int i = 0; i < num_steps; ++i)
  {
    if (i == num_steps - 1)
      Change(WAS_LAST_TRIP, 1);

    PRINT_RED(N("Hello stranger, welcome to our board!\n"));
    
    
    Change(NUMBER_ON_BOAT, 0);
    Change(LADDER_DOWN, 1);

    PRINT_CYAN(BOLD("Waiting for clearing the ladder\n"));

    Change(NUMBER_ON_LADDER, (short int)-ladder_size);
    Change(NUMBER_ON_LADDER, ladder_size);

    PRINT_MAGENTA(BOLD("Boat are full, voyage is started!!!\n")); 
    PRINT_MAGENTA(BOLD("Voyage is ended!\n"));
    
    Change(VOYAGE_STARTED, 1);
    Change(VOYAGE_ENDED, -1);

    Change(LADDER_DOWN, -1);
    
    Change(WANT_GO_COAST, 0);
    PRINT_CYAN(BOLD("Boat is empted!\n"));
    Change(VOYAGE_ENDED, 1);
    Change(WANT_GO_COAST, boat_size);
    
    if (i == num_steps - 1)
      break;
    
    Change(VOYAGE_STARTED, -1);
  }
  PRINT_RED(BOLD("Goodbye strangers!\n"));
}

int main(int argc, char** argv)
{
  
  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);
  
  short int num_pass    = 10; 
  short int boat_size   = 4;
  short int ladder_size = 2;
  int num_steps         = 5;
  
  if (argc >= 5)
  {
    num_pass    = (short int) atoi(argv[1]);
    boat_size   = (short int) atoi(argv[2]);
    ladder_size = (short int) atoi(argv[3]);
    num_steps   = atoi(argv[4]);
  }
  
  int semid = semget(IPC_PRIVATE, NUMBER_SEM, 0777); 
  TRY( semid ); 
  sembuf op{};

  Change(LADDER_DOWN, 1);
  Change(VOYAGE_ENDED, 1);
  Change(VOYAGE_STARTED, 1);
  Change(NUMBER_ON_LADDER, ladder_size);
  if (boat_size < num_pass)
  {
    Change(NUMBER_ON_BOAT, boat_size);
    Change(WANT_GO_COAST, boat_size);
  }
  else
  {
    Change(NUMBER_ON_BOAT, num_pass);
    Change(WANT_GO_COAST, num_pass);
  }

  // captain
  pid_t cpid = fork();
  TRY( cpid );
  
  if (cpid == 0)
  {
    if (boat_size < num_pass)
      captain(semid, boat_size, ladder_size, num_steps);
    else
      captain(semid, num_pass, ladder_size, num_steps);
    return 0;
  }

  // passangers
  for (int i = 0; i < num_pass; ++i)
  {
    pid_t pid = fork();
    TRY( pid );

    if (pid == 0)
    {
      passanger(semid, i + 1);
      return 0;
    }
  }
  
  return 0;
}
