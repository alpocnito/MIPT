#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <color.h>
#include <time.h>

#define FROM_LEFT_TO_RIGHT  (1)
#define FROM_RIGHT_TO_LEFT  (2)
#define PASSED              (3)

#define TRY(cmd) \
    do { if ((cmd) < 0) { \
      PRINT_RED(BOLD("Error"));\
      PRINT_WHITE(N("\n"));\
      perror(#cmd); \
      exit(EXIT_FAILURE); \
    } }while(0);
    

struct Message
{
  long type;
  long car_num;
};

void bridge(int msg_key, long num_cars)
{
  Message msg;
  PRINT_WHITE(BOLD("Bridge: started working\n"));
  
  long passed_cars = 0;
  long cur_passed_cars = 0;

  while (passed_cars < num_cars)
  {
    usleep(10000); // You can remove this line, but bridge will work too fast

    PRINT_WHITE(BOLD("Bridge: cars from left can pass\n"));
    for (cur_passed_cars = 0; cur_passed_cars < 5; ++cur_passed_cars)
      if (msgrcv(msg_key, &msg, sizeof(msg), FROM_LEFT_TO_RIGHT, IPC_NOWAIT) < 0)
      {
        // ENOMSG - not block process, if there are not Messages
        if (errno == ENOMSG)
          break;
        else
        {   
          perror("msgrcv failed on bridge\n");
          exit(0);
        }
      }
      else
      {
        msg.type = msg.car_num;
        TRY( msgsnd(msg_key, &msg, sizeof(msg), 0) );
      }
   
    // waiting for cars
    for (long i = 0; i < cur_passed_cars; ++i)
      TRY( msgrcv(msg_key, &msg, sizeof(msg), PASSED, 0) );
    
    passed_cars += cur_passed_cars;
    cur_passed_cars = 0;

/////////////////////////////////// NEXT QUEUE /////////////////////////////////////

    PRINT_WHITE(BOLD("Bridge: cars from right can pass\n"));
    for (cur_passed_cars = 0; cur_passed_cars < 5; ++cur_passed_cars)
      if (msgrcv(msg_key, &msg, sizeof(msg), FROM_RIGHT_TO_LEFT, IPC_NOWAIT) < 0)
      {
        // ENOMSG - not block process, if there are not Messages
        if (errno == ENOMSG)
          break;
        else
        {   
          perror("msgrcv failed on bridge\n");
          exit(0);
        }
      }
      else
      {
        msg.type = msg.car_num;
        TRY( msgsnd(msg_key, &msg, sizeof(msg), 0) );
      }

    // waiting for cars
    for (long i = 0; i < cur_passed_cars; ++i)
      TRY( msgrcv(msg_key, &msg, sizeof(msg), PASSED, 0) );
    
    passed_cars += cur_passed_cars;
    cur_passed_cars = 0;
  }

  PRINT_WHITE(BOLD("Bridge: went home\n"));
}


void cars(int msg_key, long car, bool from_left_to_right)
{  
  Message msg;

  msg.type    = from_left_to_right ? FROM_LEFT_TO_RIGHT : FROM_RIGHT_TO_LEFT;
  msg.car_num = car;

  if (msg.type == FROM_LEFT_TO_RIGHT)
  {
    // Came to the end of queue
    PRINT_GREEN_E(N("Car %ld arrived\n"), car);
    TRY( msgsnd(msg_key, &msg, sizeof(msg), 0) );

  
    // Waiting for bridge
    TRY( msgrcv(msg_key, &msg, sizeof(msg), car, 0) );
    PRINT_GREEN_E(N("Car %ld passed\n"), car);
  
    msg.type = PASSED;
    TRY( msgsnd(msg_key, &msg, sizeof(msg), 0) );
  }
  
  if (msg.type == FROM_RIGHT_TO_LEFT)
  {
    // Came to the end of queue
    PRINT_BLUE_E(N("Car %ld arrived\n"), car);
    TRY( msgsnd(msg_key, &msg, sizeof(msg), 0) );

  
    // Waiting for bridge
    TRY( msgrcv(msg_key, &msg, sizeof(msg), car, 0) );
    PRINT_BLUE_E(N("Car %ld passed\n"), car);
  
    msg.type = PASSED;
    TRY( msgsnd(msg_key, &msg, sizeof(msg), 0) );
  }
}

int main(int argc, char** argv)
{
  if (argc < 2)
    return 0;

  // for normal printf
  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);
  

  int num_cars_int = atoi(argv[1]);
  TRY( num_cars_int );
  long num_cars = long(num_cars_int);

  // create msgqueue
  int msg_key = msgget(IPC_PRIVATE, 0777); 
  TRY( msg_key );


  // bridge
  pid_t bpid = fork();
  TRY( bpid );

  if (bpid == 0)
  {
    bridge(msg_key, num_cars);
    return 0;
  }

  // cars
  for (long i = 5; i < num_cars + 5; ++i)
  {
    pid_t pid = fork();
    TRY( pid );

    if (pid == 0)
    {
      cars(msg_key, i + 1, i % 2);
      return 0;
    }
  }
  
  return 0;
}
