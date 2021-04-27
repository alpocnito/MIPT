#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sched.h>
#include <malloc.h>
#include <time.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/time.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include "calc.h"

#define Assert(expr) do {              \
                       if (!(expr)) {  \
                         printf("ERROR!\n"); \
                         printf("In %s:%d ----> ", __PRETTY_FUNCTION__, __LINE__); \
                         puts(#expr); \
                         perror(strerror(errno));\
                         printf("\n\n"); \
                       } } while (0); 


//////////////////////////////// INTEGRAL ///////////////////////////////////

const unsigned long long TotalStepsI = ULONG_MAX / 100;
double StartI  = 0;
double StopI = 100000;

inline double func(double x)
{
  return 1 / sqrt(x);
}

struct integral_message
{
  double start;
  double stop;
  unsigned long long n_steps;
};

//////////////////////////////// INTERNET ///////////////////////////////////
const unsigned MaxBufLen = 100;

const unsigned ServerFindPort_u = 8088;
const char*    ServerFindPort_s = "8088";

const unsigned ClientFindPort_u = 8089;
const char*    ClientFindPort_s = "8089";

const unsigned ListenPort_u = 8010;

const char*    FindMsg = "AuAuAuuu";
const unsigned FindMsgSize = sizeof(FindMsg);


const unsigned WaitNewCompTime_s = 5;
struct timeval Wait_time{WaitNewCompTime_s, 0};

void PrintAddr(struct sockaddr_in *sa)
{

  char s[INET_ADDRSTRLEN];
  if (sa->sin_family == AF_INET) 
  {
    printf("%s:%u", inet_ntop(sa->sin_family, &(sa->sin_addr), s, sizeof(s)),
                    sa->sin_port);
  }
  else
  {
    printf("Can not work with IPv6\n");
    exit(-1);
    /*printf("%s:%u", inet_ntop(sa->sa_family, &(((struct sockaddr_in6*)sa)->sin6_addr), s, sizeof(s)),
                    ((struct sockaddr_in6*)sa)->sin6_port);*/
  }
}

void PrintSocket(int sock)
{
  printf("Information about socket: %d\n", sock);

  struct sockaddr_in sin;

  socklen_t len = sizeof(sin);
  Assert(getsockname(sock, (struct sockaddr*)&sin, &len) >= 0);


  printf("--- Connected addr: ");
  PrintAddr(&sin);
  printf("\n\n");
}

void ImproveTCPSocket(int socket)
{
  const unsigned Timeout_m = 3000;
  int enable  = 1;
  int disable = 0;

  const int Probes_n = 3;
  const int Idle_s   = 2;
  const int Probes_s = 2;

  // Set time in millis that transmitted data may remain unacknowledged
  Assert(setsockopt(socket, IPPROTO_TCP, TCP_USER_TIMEOUT, &Timeout_m, sizeof(Timeout_m)) >= 0);
  
  // Send partial frames!
  Assert(setsockopt(socket, IPPROTO_TCP, TCP_CORK, &disable, sizeof(disable)) >= 0);
  
  // SEND PARTIAL FRAMES!!!!
  Assert(setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable)) >= 0);

  // Enable keepalive messages
  Assert(setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable)) >= 0);

  // Maximum number of keepalive probes tcp should send
  Assert(setsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT, &Probes_n, sizeof(Probes_n)) >= 0);

  // Time the connection needs to remain idle before TCP starts sending keepalive messages
  Assert(setsockopt(socket, IPPROTO_TCP, TCP_KEEPIDLE, &Idle_s, sizeof(Idle_s)) >= 0);

  // Time between keepalive messages
  Assert(setsockopt(socket, IPPROTO_TCP, TCP_KEEPINTVL, &Probes_s, sizeof(Probes_s)) >= 0);

  // Timeout on recv
  Assert(setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&Wait_time, sizeof(Wait_time)) >= 0);

  // Timeout on send
  Assert(setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&Wait_time, sizeof(Wait_time)) >= 0);
} 


//
// Only for sockets with SO_RCVTIMEO!!!!!
//
ssize_t SendSocketText(int socket, const char* buf, size_t count)
{
  Assert(buf);
  Assert(socket > 0);

  size_t succ_write = 0;
  while (succ_write < count)
  {
    ssize_t status = send(socket, buf + succ_write, count - succ_write, 0);
    if (status <= 0)
    {
      Assert(status > 0);
      return -1;
    }
    succ_write += (size_t)status;
  }

  return 0;
}

//
// Only for sockets with SO_RCVTIMEO!!!!!
//
ssize_t SendSocketUnsigned(int socket, unsigned long long num)
{
  char num_str[MaxBufLen];

  int str_len = snprintf(num_str, 0, "%llu", num);
  Assert(str_len > 0);
  Assert(snprintf(num_str, size_t(str_len + 1), "%llu", num) == (str_len));
  num_str[str_len] = '\0';

  return SendSocketText(socket, num_str, (size_t)str_len);
}


//
// Only for sockets with SO_RCVTIMEO!!!!!
//
ssize_t SendSocketDouble(int socket, double num)
{
  double* num_str = (double*)calloc(1, sizeof(double) + 1);
  *num_str = num;
  num_str[sizeof(double)] = '\0';

  ssize_t ret = SendSocketText(socket, (char*)num_str, sizeof(double) + 1);
  free(num_str);

  return ret;
}

void PrintBuf(const char* buf, size_t size)
{
  Assert(buf);
  
  printf("Buffer output:\nchar = int\n");
  for (size_t i = 0; i < size; ++i)
    printf("%c = %d\n", buf[i], buf[i]);
}
