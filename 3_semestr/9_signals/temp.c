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
        exit(-1); \
    } } while(0);


void handler (int num)
{}
void another_handler (int num)
{}

void child(int* child_fd, int* parent_fd, pid_t child_pid, pid_t parent_pid)
{
        char child_str[] = "child";
        char parent_str[] = "parent";
        
        close(child_fd[0]);
        
        sigset_t set;
        int sig;
        signal(SIGUSR1, &handler);
        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);
        int size = write(child_fd[1], child_str, strlen(child_str));
        if (size!=strlen(child_str)) 
        { 
            perror("write");
            exit(-1);
        }
        
        sigwait(&set, &sig); 
        printf ("CHILD:  Parent read me\n");
        
        size = read(parent_fd[0], parent_str , strlen(parent_str)); 
        TRY(size);
        printf("CHILD:  I read from parent: %s\n", parent_str );
        TRY( close(child_fd[1]));
        TRY( close(parent_fd[1]));
        TRY( close(parent_fd[0]));
     
        kill(parent_pid, SIGUSR2);
        exit(0);
}

void parent(int* child_fd, int* parent_fd, pid_t child_pid, pid_t parent_pid)
{
        char child_str[] = "child";
        char parent_str[] = "parent";
        
        close(child_fd[1]);

        int size = read(child_fd[0], child_str , strlen(child_str));
        TRY(size);
        printf("PARENT: I read from child: %s\n", child_str );
        
        kill(child_pid, SIGUSR1);
        sigset_t set1;
        int sig1;
        signal(SIGUSR2, &another_handler);
        
        sigemptyset(&set1);
        sigaddset(&set1, SIGUSR2);
        size = write(parent_fd[1], parent_str , strlen(parent_str)); 
        if(size != strlen(parent_str)) 
        {
            perror("write");
            exit(-1); 
        }

        TRY( close(child_fd[0]));
        TRY( close(parent_fd[0]));
        TRY( close(parent_fd[1]));
        sigwait(&set1, &sig1);
        printf ("PARENT: Child read me\n");

}

int main(int argc, char **argv)
{
    int child_fd[2];
    int parent_fd[2];
    
    TRY(pipe(child_fd));
    TRY(pipe(parent_fd));

    pid_t parent_pid = getpid();
    pid_t child_pid = fork();
    
    TRY(child_pid);
    // CHILD
    if (child_pid == 0) 
    {
        child(child_fd, parent_fd, child_pid, parent_pid);        
    }

    // PARENT
    if (child_pid > 0) 
    {
        parent(child_fd, parent_fd, child_pid, parent_pid);    
    }

    return 0;
}
