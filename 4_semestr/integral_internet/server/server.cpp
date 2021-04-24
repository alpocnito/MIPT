#include "integral_internet.h"

unsigned FindComputers(unsigned n_comp)
{
  if (n_comp == 0)
    return 0;

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  Assert(sockfd >= 0);
  int yes = 1;
  Assert(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes)) == 0);


  struct sockaddr_in servaddr{};
  servaddr.sin_family = AF_INET; //IPv4
  servaddr.sin_port = htons(FindPort_u);
  servaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

  //Assert(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) >= 0);
  
  Assert(sendto(sockfd, FindMsg, FindMsgSize, MSG_CONFIRM, (const struct sockaddr*) &servaddr, sizeof(servaddr)) != -1);
  
  printf("Message sent\n");

 // int recieve_len = recvfrom(sockfd, 
  return 0;
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
