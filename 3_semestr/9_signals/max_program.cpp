#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define TRY(cmd) \
  do { if (cmd < 0) {\
    perror(#cmd);\
    exit(-1);} \
  } while (0)

volatile int w;
void handler(int signal_num)
{
   w++;
}
void handler1(int signal_num)
{
   w++;
}

int main(int argc, char **argv)
{
    int fd[2];
    int fd1[2];
    TRY( pipe(fd) );
    TRY( pipe(fd1) );
    
    char child_str[] = "child";
    char parent_str[] = "parent";
    pid_t parent_pid = getpid();
    pid_t child_pid = fork();
    TRY( child_pid );
    
    // CHILD
    if (child_pid == 0) {
        sigset_t set;
        int sig;
        signal(SIGUSR1, &handler);
        close(fd[0]);
        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);
        
        int size = write(fd[1], child_str, strlen(child_str));
        if (size != strlen(child_str))
        { 
          perror("write");
          exit(-1);
        }
        close(fd[1]);

        sigwait(&set, &sig); 
        printf ("Parent had read\n");
        close(fd1[1]);

        size = read(fd1[0], parent_str, strlen(parent_str)); 
        if (size < strlen(parent_str)) 
        { 
          perror("read");
          exit(-1);
        }

        printf("Read from parent: %s\n", parent_str);
        TRY(close(fd1[0]));
        
        kill(parent_pid, SIGUSR2);
        exit(EXIT_SUCCESS);
    }


    // PARENT
    if (child_pid > 0) {
        TRY(close(fd[1]));
        int size = read(fd[0], child_str, 13 );
        TRY(size); 
        printf("Read from child : %s\n", child_str);
        
        TRY(close(fd[0])); 
        kill(child_pid, SIGUSR1);
        sigset_t set1;
        
        int sig1;
        signal(SIGUSR2, &handler1);
        TRY(close(fd1[0]));
        sigemptyset(&set1);
        sigaddset(&set1, SIGUSR2);
        size = write(fd1[1], parent_str, strlen(parent_str)); 
        if (size != strlen(parent_str)) 
        {
          perror("write");
          exit(-1); 
        }
        TRY(close(fd1[1]));
        sigwait(&set1, &sig1);
        printf ("Child read from me\n");
      }



    return (0);
}
