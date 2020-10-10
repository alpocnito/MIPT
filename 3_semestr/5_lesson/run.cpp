#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define RUNNER_CAME (N + 2)
#define RUN_IS_END  (N + 1)

struct Message
{
  long type;
};

void judge(int msg_key, int N);
void runner(int msg_key, int num_runner, int N);

void judge(int msg_key, int N)
{
  Message msg;
  printf("Judge started workin/g\n");

  // waiting for [wake up] for runner
  for (int i = 0; i < N; ++i)
  {
    if (msgrcv(msg_key, &msg, sizeof(msg), RUNNER_CAME, 0) < 0)
    {
      perror("msgrcv");
      exit(EXIT_FAILURE);
    }
  }
  
  printf("Start of running\n");
  for (int i = 0; i < N; ++i)
  {
    // [start of running] for runner
    msg.type = i + 1;
    if (msgsnd(msg_key, &msg, sizeof(msg), 0) < 0)
    {
      perror("msgrcv");
      exit(EXIT_FAILURE);
    }
  
    // [stop of running] for runner
    if (msgrcv(msg_key, &msg, sizeof(msg), i + 1, 0) < 0)
    {
      perror("msgrcv");
      exit(EXIT_FAILURE);
    }
  }
  
  // [stop of run] for runner
  printf("Stop of running\n");
  for (int i = 0; i < N; ++i)
  {
    msg.type = RUN_IS_END;
    if (msgsnd(msg_key, &msg, sizeof(msg), 0) < 0)
    {
      perror("msgrcv");
      exit(EXIT_FAILURE);
    }
  }
  printf("Judge went home\n");
}


void runner(int msg_key, int num_runner, int N)
{  
  Message msg;


  // [wake up] for judge
  msg.type = RUNNER_CAME;
  printf("%d waked up\n", num_runner);
  if (msgsnd(msg_key, &msg, sizeof(msg), 0) < 0)
  {
    perror("msgsnd");
    exit(EXIT_FAILURE);
  }

  // [start of running] for judge
  if (msgrcv(msg_key, &msg, sizeof(msg), num_runner, 0) < 0)
  {
    perror("msgrcv");
    exit(EXIT_FAILURE);
  }

  printf("%d runned up\n", num_runner);
  msg.type = num_runner;

  // [stop of running] for judge
  if (msgsnd(msg_key, &msg, sizeof(msg), 0) < 0)
  {
    perror("msgsnd");
    exit(EXIT_FAILURE);
  }

  // [stop of run] for judge 
  if (msgrcv(msg_key, &msg, sizeof(msg), RUN_IS_END, 0) < 0)
  {
    perror("msgrcv");
    exit(EXIT_FAILURE);
  }
  
  printf("%d went home\n", num_runner);
}

int main(int argc, char** argv)
{
  if (argc < 2)
    return 0;

  char print_buf[BUFSIZ];
  setvbuf(stdout, print_buf, _IOLBF, BUFSIZ);

  int N = atoi(argv[1]);
  if (N < 1)
    return 0;
  
  int msg_key = msgget(IPC_PRIVATE, 0777); 
  if (msg_key < 0)
  {
    perror("msgget");
    exit(EXIT_FAILURE);
  }

  // judge
  pid_t jpid = fork();
  if (jpid < 0)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if (jpid == 0)
  {
    judge(msg_key, N);
    return 0;
  }

  // runners
  for (int i = 0; i < N; ++i)
  {
    pid_t pid = fork();
    if (pid < 0)
    {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
      runner(msg_key, i + 1, N);
      return 0;
    }
  }
  
  return 0;
}
