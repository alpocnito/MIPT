#include "integral_internet.h"


void print_in_addr(struct sockaddr *sa)
{

  char s[INET_ADDRSTRLEN];
  if (sa->sa_family == AF_INET) 
  {
    printf("%s:%u", inet_ntop(sa->sa_family, &(((struct sockaddr_in*)sa)->sin_addr), s, sizeof(s)),
                    ((struct sockaddr_in*)sa)->sin_port);
  }
  else
  {
    printf("%s:%u", inet_ntop(sa->sa_family, &(((struct sockaddr_in6*)sa)->sin6_addr), s, sizeof(s)),
                    ((struct sockaddr_in6*)sa)->sin6_port);
  }
}

int main(int argc, char** argv)
{

  struct sockaddr_storage their_addr;
  struct addrinfo *servinfo;
  
  struct addrinfo hints{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  Assert(getaddrinfo(NULL, FindPort_s, &hints, &servinfo) == 0);
  
  int sockfd = 0;
  struct addrinfo* p = NULL;
  for (p = servinfo; p != NULL; p = p->ai_next)
  {
    Assert( (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) != -1);  
    Assert(bind(sockfd, p->ai_addr, p->ai_addrlen) != -1);
    break;
  }
  Assert(p);

  freeaddrinfo(servinfo);

  printf("Listener waiting for recvfrom:\n");

  socklen_t addr_len = sizeof(their_addr);
  char buf[FindMsgSize];
  ssize_t numbytes = 0;
  Assert((numbytes = recvfrom(sockfd, buf, FindMsgSize-1, 0, (struct sockaddr*)&their_addr, &addr_len)) != -1);


  printf("listener: got packet from ");
  print_in_addr((struct sockaddr*)&their_addr);
  printf("\n");

  printf("listener: packet is %ld bytes long\n", numbytes);
  printf("listener: packet contains \"%s\"\n", buf);

  close(sockfd);


  return 0;
}
