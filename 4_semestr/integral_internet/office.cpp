#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#include <sched.h>
#include <malloc.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

const unsigned Timeout_s = 10;
const unsigned Port = 8088;

unsigned FindComputers(unsigned n_comp)
{
  if (n_comp == 0)
    return 0;

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  Assert(sockfd >= 0);

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(Port);
  
  Assert(bind(sockfd, (const struct sockaddr *)servaddr, sizeof(servaddr)) >= 0);
  
  Assert(sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr*) &servaddr, sizeof(servaddr));
  
  printf("Message sent\n");

  int recieve_len = recvfrom(sockfd, 

}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    printf("Incorrect input\nRun with desireable number of computers\n");
    return -1;
  }
  unsigned n_comp = unsigned(atoi(argv[1]));

  unsigned n_comp_find = FindComputers(n_comp);  

  return 0;
}
