//
// Сделаны все доп. задания
//

#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <semaphore.h>
#include <time.h>

#define handle_error_en(er, msg) \
	do {errno = er; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
	do {perror(msg); exit(EXIT_FAILURE); } while (0)


void client_job(unsigned size, unsigned rank, unsigned N, double step);
void server_job(unsigned size, unsigned rank, unsigned N, double step);
void* thread_func(void* v_num);
double func(double start, double stop, double step);

const double Start = 1;
const double EPS = 0.00000001;

// for thread_func
sem_t sem_calloc;
double** args;
sem_t* sem_args;
sem_t* sem_ans;


struct Thread_args
{
    unsigned rank;
    unsigned np;
    unsigned N;
    double step;
};

int main(int argc, char** argv)
{
    struct timespec begin, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &begin);

	assert(argc == 4);
    char* tmp;
	unsigned int np    = (unsigned int)atoi(argv[1]);
	unsigned int N     = (unsigned int)atoi(argv[2]);
	double       step  =               strtod(argv[3], &tmp);
	assert(N > 0);
	assert(np > 0);
	assert(step > 0);

	// special cases
	if (N == 1 or np == 1)
	{
		printf("Ans: %lg\n", func(Start, Start + N*step, step));
        
        clock_gettime(CLOCK_REALTIME, &end);
        elapsed = double(end.tv_sec - begin.tv_sec);
        elapsed += double(end.tv_nsec - begin.tv_nsec) / 1000000000.0;
        printf("Time for program: %lg\n", elapsed);
        return 0;
	}

	if (np >= N + 2)
	{
		np = N + 1;
	}
    
   
    // init semaphore for thread_func
    if (sem_init(&sem_calloc, 0, 0) == -1)
        handle_error("sem_init");
    

    pthread_t*   thread_ids  = (pthread_t*)   calloc(np, sizeof(thread_ids[0]));
    Thread_args* thread_args = (Thread_args*) calloc(np, sizeof(thread_args[0]));
        
	for (unsigned j = 0; j < np; ++j)
	{
		thread_args[j].rank = j;
		thread_args[j].np = np;
		thread_args[j].N = N;
		thread_args[j].step = step;
		
		int ret = pthread_create(&(thread_ids[j]), NULL, thread_func, (void*)(&(thread_args[j])));
		if (ret != 0)
			handle_error_en(ret, "pthread_create");
	}
	
	for (unsigned j = 0; j < np; ++j)
	{
		int ret = pthread_join(thread_ids[j], NULL);
		if (ret != 0)
			handle_error_en(ret, "pthread_join");
	}
    
    free(thread_ids);
	free(thread_args);

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = double(end.tv_sec - begin.tv_sec);
    elapsed += double(end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    printf("Time for program: %lg\n", elapsed);
    return 0;
}


double func(double start, double stop, double step)
{
	assert(start < stop);
	assert(step > 0);

	double ans = 0;
    while (start + EPS < stop)
    {
        //printf("    start: %lg; step: %lg; stop: %lg\n", start, step, stop);
        ans += start * step;
        start += step;
    }

	return ans;
}

void* thread_func(void* v_num)
{
    struct timespec begin, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &begin);

    unsigned rank = ((Thread_args*)v_num)->rank;
    unsigned np   = ((Thread_args*)v_num)->np;
    unsigned N    = ((Thread_args*)v_num)->N;
    double   step = ((Thread_args*)v_num)->step;

    if (rank == 0)
    {
        args = (double**) calloc(np, sizeof(args[0]));
        sem_args = (sem_t*) calloc(np, sizeof(sem_args[0]));
        sem_ans = (sem_t*) calloc(np, sizeof(sem_ans[0]));
        
        for (unsigned i = 0; i < np; ++i)
        {
            args[i] = (double*) calloc(2, sizeof(args[0][0]));
            if (sem_init(&(sem_args[i]), 0, 0) == -1)
                handle_error("sem_init");
            if (sem_init(&(sem_ans[i]), 0, 0) == -1)
                handle_error("sem_init");
        }

        for (unsigned i = 0; i < np; ++i)
            if (sem_post(&sem_calloc) != 0)
                handle_error("sem_post");

        ////
	    
        server_job(np, rank, N, step);
        
        ////
        
        for (unsigned i = 0; i < np; ++i)
        {
            if (sem_destroy(&(sem_args[i])) == -1)
                handle_error("sem_destroy");
            if (sem_destroy(&(sem_ans[i])) == -1)
                handle_error("sem_destroy");
            free(args[i]);
        }
    
        free(args);
        free(sem_ans);
        free(sem_args);
    }
    else
    {
        if (sem_wait(&sem_calloc) != 0)
            handle_error("sem_wait");
        
        /*
        int ret = 0;
        if (sem_getvalue(&sem_calloc, &ret) != 0)
            handle_error("sem_getvalue");
        printf("Value %d\n", ret);
        */

		client_job(np, rank, N, step);
    }


    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = double(end.tv_sec - begin.tv_sec);
    elapsed += double(end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    printf("Time for rank %2u: %lg\n", rank, elapsed);
   	return NULL;
}


void server_job(unsigned size, unsigned rank, unsigned N, double step)
{
	assert(size > 1);
	assert(rank == 0);
	assert(N > 1);
    assert(step > 0);

	for (unsigned int i = 0; i < size - 1; ++i)
	{
		// Начала и конец отсчета для каждого процессора
		if (i == 0)
			args[i][0] = Start;
		else
			args[i][0] = args[i-1][1];
			
		if (i == size - 2)
			args[i][1] = Start + step*N;
		else
			args[i][1] = args[i][0] + double(N / (size - 1)) * step;
	    
        sem_post(&(sem_args[i]));

		//printf("SERVER Process %2u has start at %3.4lg, end at %3.4lg\n", i, args[i][0], args[i][1]); 
	}
    
	// Wait clients
    double ans = 0;
	for (unsigned int i = 0; i < size - 1; ++i)
    {
        //printf("SERVER Wait for %u\n", i);
        if (sem_wait(&(sem_ans[i])) == -1)
            handle_error("sem_wait");

        ans += args[i][0];
    }

	printf("Answer: %lg\n", ans);
	return;
}

void client_job(unsigned size, unsigned rank, unsigned N, double step)
{
	assert(size > 1);
	assert(rank > 0);
	assert(N > 1);
	assert(step > 0);

    //printf("CLIENT Process %u is waiting\n", rank-1);
    if (sem_wait(&(sem_args[rank-1])) == -1)
        handle_error("sem_wait");
	
	// client counts
	args[rank-1][0] = func(args[rank-1][0], args[rank-1][1], step);
    
    //printf("CLIENT Process %u is ending\n", rank-1);
    if (sem_post(&(sem_ans[rank-1])) == -1)
        handle_error("sem_post");

	return;
}
