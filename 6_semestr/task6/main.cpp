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


void client_job(unsigned size, unsigned rank, unsigned N);
void server_job(unsigned size, unsigned rank, unsigned N);
void* thread_func(void* v_num);
unsigned sum (unsigned int N);

// for thread_func
sem_t sem_calloc;
unsigned** args;
sem_t* sem_args;
sem_t* sem_ans;

int main(int argc, char** argv)
{
    struct timespec begin, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &begin);


	assert(argc == 3);
	unsigned int np = (unsigned int)atoi(argv[1]);
	unsigned int N  = (unsigned int)atoi(argv[2]);
	assert(N > 0);
	assert(np > 0);

	// special cases
	if (N == 1)
	{
		printf("Ans: %u\n", 1);
        clock_gettime(CLOCK_REALTIME, &end);
        elapsed = double(end.tv_sec - begin.tv_sec);
        elapsed += double(end.tv_nsec - begin.tv_nsec) / 1000000000.0;
        printf("Time for program: %lg\n", elapsed);
        return 0;
	}

	if (np == 1)
	{
		printf("Ans: %u\n", sum(N));
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
    

    pthread_t* thread_ids  = (pthread_t*)calloc(np, sizeof(thread_ids[0]));
	unsigned** thread_args = (unsigned**)calloc(np, sizeof(thread_args[0]));
        
	for (unsigned j = 0; j < np; ++j)
	{
        thread_args[j] = (unsigned*) calloc(3, sizeof(thread_args[0][0]));
		thread_args[j][0] = j;
		thread_args[j][1] = np;
		thread_args[j][2] = N;
		
		int ret = pthread_create(&(thread_ids[j]), NULL, thread_func, (void*)(thread_args[j]));
		if (ret != 0)
			handle_error_en(ret, "pthread_create");
	}
	
	for (unsigned j = 0; j < np; ++j)
	{
		int ret = pthread_join(thread_ids[j], NULL);
		if (ret != 0)
			handle_error_en(ret, "pthread_join");
        free(thread_args[j]);
	}
	
	
    
    free(thread_ids);
	free(thread_args);

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = double(end.tv_sec - begin.tv_sec);
    elapsed += double(end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    printf("Time for program: %lg\n", elapsed);
    return 0;
}


unsigned sum (unsigned int N)
{
	assert(N > 0);

	unsigned ans = 0;
	for (unsigned int i = 1; i <= N; ++i)
		ans += i;

	return ans;
}

void* thread_func(void* v_num)
{
    struct timespec begin, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &begin);

    unsigned rank = ((unsigned*)v_num)[0];
    unsigned np   = ((unsigned*)v_num)[1];
    unsigned N    = ((unsigned*)v_num)[2];
    
    if (rank == 0)
    {
        args = (unsigned**) calloc(np, sizeof(args[0]));
        for (unsigned i = 0; i < np; ++i)
            args[i] = (unsigned*) calloc(2, sizeof(args[0][0]));
    
        sem_args = (sem_t*) calloc(np, sizeof(sem_args[0]));
        for (unsigned i = 0; i < np; ++i)
            if (sem_init(&(sem_args[i]), 0, 0) == -1)
                handle_error("sem_init");
 
        sem_ans = (sem_t*) calloc(np, sizeof(sem_ans[0]));
        for (unsigned i = 0; i < np; ++i)
            if (sem_init(&(sem_ans[i]), 0, 0) == -1)
                handle_error("sem_init");
        
        for (unsigned i = 0; i < np; ++i)
            if (sem_post(&sem_calloc) != 0)
                handle_error("sem_post");

        ////
	    
        server_job(np, rank, N);
        
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

		client_job(np, rank, N);
    }


    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = double(end.tv_sec - begin.tv_sec);
    elapsed += double(end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    printf("Time for rank %2u: %lg\n", rank, elapsed);
   	return NULL;
}


void server_job(unsigned size, unsigned rank, unsigned N)
{
	assert(size > 1);
	assert(rank == 0);
	assert(N > 1);

	for (unsigned int i = 0; i < size - 1; ++i)
	{
		// Начала и конец отсчета для каждого процессора
		if (i == 0)
			args[i][0] = 1;
		else
			args[i][0] = args[i-1][1] + 1;
			
		if (i == size - 2)
			args[i][1] = N;
		else
			args[i][1] = args[i][0] - 1 + N / (size - 1);
	    
        sem_post(&(sem_args[i]));

		//printf("SERVER Process %2u has start at %6u, end at %6u\n", i, args[i][0], args[i][1]); 
	}
    
	// Wait clients
    unsigned int ans = 0;
	for (unsigned int i = 0; i < size - 1; ++i)
    {
        //printf("SERVER Wait for %u\n", i);
        if (sem_wait(&(sem_ans[i])) == -1)
            handle_error("sem_wait");

        ans += args[i][0];
    }

	printf("Answer: %u\n", ans);
	return;
}

void client_job(unsigned size, unsigned rank, unsigned N)
{
	//double start_time = MPI_Wtime();

	assert(size > 1);
	assert(rank > 0);
	assert(N > 1);

    //printf("CLIENT Process %u is waiting\n", rank-1);
    if (sem_wait(&(sem_args[rank-1])) == -1)
        handle_error("sem_wait");
	
	// client counts
	unsigned int ans = 0;
	for (unsigned int i = args[rank-1][0]; i <= args[rank-1][1]; ++i)
		ans += i;
	args[rank-1][0] = ans;
    
    //printf("CLIENT Process %u is ending\n", rank-1);
    if (sem_post(&(sem_ans[rank-1])) == -1)
        handle_error("sem_post");

	//printf("%2d: PROC TIME IN SEC: %lg\n", rank, MPI_Wtime() - start_time);	
	return;
}
