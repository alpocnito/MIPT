#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define TRY(cmd) \
    if ((cmd) != 0) { \
      perror(#cmd); \
      exit(EXIT_FAILURE); \
    }

long long count = 0;

struct Thread_arguments
{
	long long numCount;  
	long long numThread;

	pthread_mutex_t cs;
};

void* A_Algorithm(void* args)
{
	Thread_arguments* data = (Thread_arguments*) args;

	for (long long i = 0; i < data->numCount/data->numThread; ++i)
		count++;

	return NULL;
}

void* B_Algorithm(void* args)
{
	Thread_arguments* data = (Thread_arguments*) args;

	for (long long i = 0; i < data->numCount/data->numThread; ++i)
	{
	  TRY(pthread_mutex_lock(&(data->cs)));
		count++;
		TRY(pthread_mutex_unlock(&(data->cs)));
	}
	
	return NULL;
}

void* C_Algorithm(void* args)
{
	Thread_arguments* data = (Thread_arguments*) args;

	TRY(pthread_mutex_lock(&(data->cs)));
  for (long long i = 0; i < data->numCount/data->numThread; ++i)
		count++;	
	TRY(pthread_mutex_unlock(&(data->cs)));

	return NULL;
}

void* D_Algorithm(void* args)
{
	Thread_arguments* data = (Thread_arguments*) args;

	long long local_count = 0;
	for (long long i = 0; i < data->numCount/data->numThread; ++i)
		local_count++;

	TRY(pthread_mutex_lock(&(data->cs)));
	count += local_count;
	TRY(pthread_mutex_unlock(&(data->cs)));

	return NULL;
}

int main(int argc, char** argv)
{
	if (argc != 4)
	    return 0;
    
  Thread_arguments arguments;
  arguments.numCount  = atoll(argv[1]);
  arguments.numThread = atoll(argv[2]);
  char algorithm      = argv[3][0];

	pthread_t* threads = (pthread_t*) calloc((size_t)arguments.numThread, sizeof(threads[0]));
	pthread_mutex_init(&(arguments.cs), NULL); 
    
  void* (*function) (void*);    

	switch(algorithm)
	{
	    case 'A':
			function = &A_Algorithm;
			break;
		case 'B':
			function = &B_Algorithm;
			break;
        case 'C':
			function = &C_Algorithm;
			break;
        case 'D':
			function = &D_Algorithm;
			break;

		default:
			printf("Unknown algorythm\n");
			return 0;
	}

////////////////////////////////////////////
    timespec  tp{};
  
    clock_gettime(CLOCK_MONOTONIC, &tp);
    long int old_nanosecs = tp.tv_nsec;
    long int old_seconds  = tp.tv_sec;    
////////////////////////////////////////////



  for (long long i = 0; i < arguments.numThread; i++)
		TRY(pthread_create(threads + i, NULL, function, &arguments));

	for (long long i = 0; i < arguments.numThread; i++)
		TRY(pthread_join(threads[i], NULL));
			
  printf("Count: %lld\n", count);

////////////////////////////////////////////
    clock_gettime(CLOCK_MONOTONIC, &tp);    
    long int new_nanosecs = tp.tv_nsec;   
    long int new_seconds  = tp.tv_sec; 

    printf("Milliseconds: %lg\n", double(new_seconds - old_seconds) * 1000 + double(new_nanosecs - old_nanosecs) / 1000000);   
////////////////////////////////////////////
  
  free(threads);
	return 0;
}

