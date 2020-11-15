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
size_t cur_bit  = 0;


///////////////////////////////////// RECEIVER //////////////////////////////////
void Receive_0(int)
{
  if (cur_bit == 7)
  {
    cur_bit = 0;
    cur_byte++;
  }
  else
    cur_bit++;
  
  kill(sender_pid, 6);
}

void Receive_1(int)
{
  output[cur_byte] |= (1 << cur_bit); 

  if (cur_bit == 7)
  {
    cur_bit = 0;
    cur_byte++;
  }
  else
    cur_bit++;

  kill(sender_pid, 6);
}
/////////////////////////////////////////////////////////////////////////////////



///////////////////////////////// SENDER + RECIEVER /////////////////////////////
void Kill(int)
{
  printf("output: %s", output);
  free(input);
  free(output);
  exit(0);
}
/////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////// SENDER ///////////////////////////////////
void Send(int) {}
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
    act.sa_handler = Receive_0;
    act.sa_flags = 0;
    TRY( sigaction(7, &act, NULL) )
    
    act.sa_handler = Receive_1;
    TRY( sigaction(8, &act, NULL) )
    
    act.sa_handler = Kill;
    TRY( sigaction(29, &act, NULL) )
    
    sigset_t mask;
    sigemptyset(&mask);
    while (1)
      sigsuspend(&mask);
  }
  else
  {
    struct sigaction act{};
    act.sa_handler = Send;
    act.sa_flags = 0;
    TRY( sigaction(6, &act, NULL) );
    
    for (size_t bit = 0; bit < (size_t)input_size * 8; ++bit)
    {
      sigset_t mask;
      TRY( sigfillset(&mask) )
      TRY( sigprocmask(SIG_BLOCK, &mask, NULL) )
      TRY( sigemptyset(&mask) )
  
      /////////////// logic //////////////////
      char bit_val  = input[bit/8] & (1 << (bit % 8));
    
      if (bit_val == 0)
        TRY( kill(receiver_pid, 7) )
      else
        TRY( kill(receiver_pid, 8) )
      ////////////////////////////////////////
     
      sigsuspend(&mask);
    }
    
    TRY( kill(receiver_pid, 29) )
  }
   
  return 0;
}
