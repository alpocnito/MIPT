#include <stdio.h>
#include <color.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define TRY(cmd) \
    do { if ((cmd) < 0) { \
      PRINT_RED(BOLD("Error"));\
      PRINT_WHITE(N("\n"));\
      perror(#cmd); \
      exit(EXIT_FAILURE); \
    } }while(0);

pid_t sender_pid = 0;
pid_t receiver_pid = 0;

char* input = NULL;
char* output = NULL;
size_t cur_byte = 0;


///////////////////////////////////// RECEIVER //////////////////////////////////
void Receive(int, siginfo_t* info, void*)
{
  output[cur_byte++] = info->si_value.sival_int;
}

void Kill(int, siginfo_t*, void*)
{
  printf("output: %s", output);
  free(input);
  free(output);
  exit(0);
}
/////////////////////////////////////////////////////////////////////////////////


int main()
{
  size_t tmp = 0;
  ssize_t input_size = getline(&input, &tmp, stdin);
  TRY(input_size);
  output = (char*)calloc(input_size + 1, sizeof(output[0])); 

  receiver_pid = getpid();
  sender_pid   = fork();

  if (sender_pid != 0)
  {
    struct sigaction act{};
    act.sa_sigaction = Receive;
    act.sa_flags = SA_SIGINFO;
    TRY( sigaction(40, &act, NULL) )
    
    act.sa_sigaction = Kill;
    TRY( sigaction(41, &act, NULL) )
    
    sigset_t mask;
    int sig = 0;
    sigemptyset(&mask);
    while (1)
      if (sigwait(&mask, &sig) > 0)
      {
        perror("sigwait");
        exit(0);
      }
  }
  else
  {
    sigval value{};
    for (size_t byte = 0; byte < (size_t)input_size; ++byte)
    {
      value.sival_int = input[byte];
      TRY( sigqueue(receiver_pid, 40, value) )
    }
    
    TRY( sigqueue(receiver_pid, 41, value) )
  }
   
  return 0;
}
