#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

volatile size_t ended_childs = 0;
const    size_t MaxCommandSize = 1024;
const    char   FileName[] = "files.txt";

struct thread_arg
{
    unsigned delay;
    char command[MaxCommandSize];
};

void *mythread(void *arg) 
{
    sleep(((struct thread_arg*)arg)->delay);
    const char *args = ((struct thread_arg*)arg)->command;
    
    pid_t child = fork();
    if (child == 0) 
    {
      printf("%s\n", args);
      execlp(args, args, NULL);
    } 
    wait(NULL);

    ended_childs += 1;
    return NULL;
}

int main()
{
    FILE* file;
    if ((file = fopen(FileName, "r")) == NULL) 
    {
        printf("No file");
        exit(1);
    }

    
    size_t commands_counter = 0;
    struct thread_arg arg;
    while (fscanf(file, "%u %s", &(arg.delay), arg.command) != EOF) 
    {
        pthread_t temp;
        int stat = pthread_create(&temp, NULL, mythread, &arg);
        if (stat < 0)
        {
          perror("pthread_create");
          exit(-1);
        }

        commands_counter++;
    }
    
    // wait for childs
    while(commands_counter != ended_childs);
    
    return 0;
}
