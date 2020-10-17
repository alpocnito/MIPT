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

#define TRY(cmd) \
    if ((cmd) < 0) { \
      perror(#cmd); \
      exit(EXIT_FAILURE); \
    }

#define Change(SEM_NAME, VAL) \
    op = {SEM_NAME, VAL, 0};  \
    TRY( semop(semid, &op, 1) );

void passanger(int semid, int pass_name)
{
  sembuf op{};
  while (1)
  {
    Change(LADDER_DOWN, 0);
    
    Change(NUMBER_ON_BOAT, -1);
    Change(NUMBER_ON_LADDER, -1);
    PRINT_GREEN_E(N("Passangers %d on ladder\n"), pass_name);
    Change(NUMBER_ON_LADDER, 1);
    PRINT_GREEN_E(N("Passangers %d on boat\n"), pass_name);
    
    Change(LADDER_DOWN, 0);
    Change(NUMBER_ON_LADDER, -1);
    PRINT_GREEN_E(N("Passangers %d on ladder\n"), pass_name);
    Change(NUMBER_ON_LADDER, 1);
    PRINT_GREEN_E(N("Passangers %d on coast\n"), pass_name);
    Change(NUMBER_ON_BOAT, -1);
    Change(WANT_GO_COAST, -1);
  }
}

void captain(int semid, short int boat_size, short int ladder_size, int num_steps)
{ 
  sembuf op{};
  for (int i = 0; i < num_steps; ++i)
  {
    PRINT_RED(N("Hello stranger, welcome to our board!\n"));
    
    Change(LADDER_DOWN, -1);
    Change(NUMBER_ON_BOAT, 0);
    Change(LADDER_DOWN, 1);

    Change(NUMBER_ON_BOAT, boat_size);
    PRINT_CYAN(BOLD("Waiting for clearing the ladder\n"));

    Change(NUMBER_ON_LADDER, (short int)-ladder_size);
    Change(NUMBER_ON_LADDER, ladder_size);

    PRINT_MAGENTA(BOLD("Boat are full, voyage is started!!!\n")); 
    PRINT_MAGENTA(BOLD("Voyage is ended!\n"));

    Change(CAN_OUT, -1);
    Change(NUMBER_ON_BOAT, 0);
    Change(CAN_OUT, 1);
    
    Change(NUMBER_ON_BOAT, boat_size);
    Change(WANT_GO_COAST, 0);
    PRINT_CYAN(BOLD("Boat is empted!\n"));
  }
}

int main(int argc, char** argv)
{
  
  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);
  
  int num_pass          = 10; 
  short int boat_size   = 4;
  short int ladder_size = 2;
  int num_steps         = 5;
  
  if (argc >= 5)
  {
    num_pass    = atoi(argv[1]);
    boat_size   = (short int) atoi(argv[2]);
    ladder_size = (short int) atoi(argv[3]);
    num_steps   = atoi(argv[4]);
  }
  
  int semid = semget(IPC_PRIVATE, 10, 0777); 
  TRY( semid ); 
  sembuf op{};

  Change(CAN_OUT, 1);
  Change(CAN_IN, 1);
  Change(NUMBER_ON_LADDER, ladder_size);
  if (boat_size < num_pass)
    Change(NUMBER_ON_BOAT, boat_size);
  else
    Change(NUMBER_ON_BOAT, num_pass);

  // captain
  pid_t cpid = fork();
  TRY( cpid );
  
  if (cpid == 0)
  {
    captain(semid, boat_size, ladder_size, num_steps);
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
