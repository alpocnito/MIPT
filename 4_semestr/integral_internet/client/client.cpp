#include "integral_internet.h"

struct sockaddr_in* WaitServer()
{
  struct sockaddr_storage* their_addr = (struct sockaddr_storage*) calloc(1, sizeof(struct sockaddr_storage));
  Assert(their_addr);

  struct addrinfo hints{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo* servinfo;
  Assert(getaddrinfo(NULL, ServerFindPort_s, &hints, &servinfo) == 0);
  
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
  // Timeout on recv
  Assert(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&Wait_time, sizeof(Wait_time)) >= 0);

  socklen_t addr_len = sizeof(their_addr);
  char buf[FindMsgSize];
  ssize_t numbytes = 0;
  Assert((numbytes = recvfrom(sockfd, buf, FindMsgSize-1, 0, (struct sockaddr*)their_addr, &addr_len)) > 0);
  
  if (numbytes == -1)
  {
    printf("There are no server(\n");
    close(sockfd);
    exit(-1);
  }

  printf("listener: packet is %ld bytes long\n", numbytes);
  printf("listener: packet contains \"%s\"\n", buf);

  close(sockfd);
  
  return (struct sockaddr_in*)their_addr;
}


int ConnectToServer(sockaddr_in* servaddr)
{
  Assert(servaddr);

  int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
  Assert(tcp_socket > 0);

  int enable = 1;
  Assert(setsockopt(tcp_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) >= 0);
  
  struct sockaddr_in tcp_servaddr{};
  tcp_servaddr.sin_family = AF_INET;
  tcp_servaddr.sin_port   = htons(ListenPort_u);
  tcp_servaddr.sin_addr   = servaddr->sin_addr;

  Assert(connect(tcp_socket, (struct sockaddr*) &tcp_servaddr, sizeof(tcp_servaddr)) >= 0);
  ImproveTCPSocket(tcp_socket);

  return tcp_socket;
}

void SendThreadInformation(int tcp_socket)
{
  printf("\nReady to send Thread information\n");
  
  cpu_info_t cpu_info{};
  GetCpuInfo(&cpu_info);

  SendSocketUnsigned(tcp_socket, cpu_info.virtual_cpu_num);
  printf("Sended from:\n");
  PrintSocket(tcp_socket);

  FreeCpuInfo(&cpu_info);
}

struct integral_message* ReceiveJob(int sock)
{
  struct integral_message* job = (struct integral_message*) calloc(1, sizeof(struct integral_message));
  Assert(job);

  int recv_bytes = (int)recv(sock, (void*)job, sizeof(struct integral_message), 0);
  Assert(recv_bytes > 0);
  Assert((size_t)recv_bytes == sizeof(struct integral_message));
  
  return job;
}

double DoJob(struct integral_message* job)
{
  Assert(job);

  cpu_info_t cpu_info = {};
  GetCpuInfo(&cpu_info);
  
  double ans = 0;
  DistributeThreads(&cpu_info, cpu_info.virtual_cpu_num, &ans);
  FreeCpuInfo(&cpu_info);

  return ans;
} 

int main(int argc, char** argv)
{
  struct sockaddr_in* servaddr = WaitServer(); 
  
  printf("listener: got packet from ");
  PrintAddr(servaddr);
  printf("\n");

  int tcp_socket = ConnectToServer(servaddr);
  SendThreadInformation(tcp_socket);
  
  struct integral_message* job = ReceiveJob(tcp_socket);
  printf("Get job:\nStart: %lg\nStop: %lg\nN_steps: %llu\n----------------\n", \
         job->start, job->stop, job->n_steps);

  double ans = DoJob(job);
  printf("I calculated: %lg\n", ans);
  
  SendSocketDouble(tcp_socket, ans);

  sleep(5);
  
  close(tcp_socket);
  free(servaddr); 
  free(job);
  return 0;
}
