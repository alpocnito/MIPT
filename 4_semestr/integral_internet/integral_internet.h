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

#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/time.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define Assert(expr) do {              \
                       if (!(expr)) {  \
                         printf("ERROR!\n"); \
                         printf("In %s:%d ----> ", __PRETTY_FUNCTION__, __LINE__); \
                         puts(#expr); \
                         perror(strerror(errno));\
                         printf("\n\n"); \
                       } } while (0); 

const unsigned Timeout_s = 10;
const unsigned FindPort_u = 8088;
const char*    FindPort_s = "8088";

const char*    FindMsg = "AuAuAuuuuu";
const unsigned FindMsgSize = sizeof(FindMsg);

