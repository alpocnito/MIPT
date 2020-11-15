#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define TRY(cmd) \
    if ((cmd) < 0) { \
      perror(#cmd); \
      exit(EXIT_FAILURE); \
    }

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
  if (getpid() == sender_pid)
    exit(0);

  printf("output: %s", output);
  free(input);
  free(output);
  exit(0);
}
/////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////// SENDER ///////////////////////////////////
void Send(int)
{
  if (input[cur_byte] != '\0')
  {
    char byte = input[cur_byte];
    char bit  = byte & (1 << cur_bit);
    
    if (cur_bit == 7)
    {
      cur_bit = 0;
      cur_byte++;
    }
    else
      cur_bit++;
   
    if (bit == 0)
      kill(receiver_pid, 7);
    else
      kill(receiver_pid, 8);
  }
  else
    kill(receiver_pid, 29);
}
/////////////////////////////////////////////////////////////////////////////////

int main()
{
  size_t tmp = 0;
  ssize_t input_size = getline(&input, &tmp, stdin);
  TRY(input_size);
  output = (char*)calloc(input_size + 1, sizeof(output[0])); 

  receiver_pid = getpid();
  sender_pid = fork();

  if (sender_pid != 0)
  {
     signal(7, Receive_0);
     signal(8, Receive_1);
     signal(29, Kill);
     while (1)
       pause();
  }
  else
  {
    signal(29, Kill);
    signal(6, Send);
    Send(0);
    while (1)
      pause();
  }
   
  return 0;
}
