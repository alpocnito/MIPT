//
// Сделаны все доп. задания
//

#include "mpi.h"
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"

#ifdef DEBUG
	#define Printf(...) printf(__VAR__ARGS__);
#else
	#define Printf(...) ;
#endif

void server_job(int size, int rank, unsigned int N)
{
	assert(size > 1);
	assert(rank == 0);
	assert(N > 1);
	
	// Requests for Isend
	MPI_Request* requests = (MPI_Request*)calloc((unsigned int)(size - 1), sizeof(requests[0]));
	assert(requests);

	// Buffers with data
	unsigned int** buffers = (unsigned int**)calloc((size_t)size - 1, sizeof(buffers[0]));
	assert(buffers);

	for (unsigned int i = 0; i < size - 1; ++i)
	{
		// buffer for each process
		buffers[i] = (unsigned int*)calloc(2, sizeof(buffers[0][0]));
		assert(buffers[i]);
			
		// Начала и конец отсчета для каждого процессора
		if (i == 0)
			buffers[i][0] = 1;
		else
			buffers[i][0] = buffers[i-1][1] + 1;
			
		if (i == size - 2)
			buffers[i][1] = N;
		else
			buffers[i][1] = buffers[i][0] - 1 + N / (unsigned int)(size - 1);
		
		Printf("Process %2u has start at %6u, end at %6u\n", i, buffers[i][0], buffers[i][1]); 

		assert(MPI_Isend(buffers[i], 2, MPI_UNSIGNED, int(i + 1), 0, MPI_COMM_WORLD, &(requests[i])) == MPI_SUCCESS);
	}
	
	// Wait untile Isend ends
	for (unsigned int i = 0; i < size - 1; ++i)
		assert(MPI_Wait(&(requests[i]), MPI_STATUS_IGNORE) == MPI_SUCCESS);

	unsigned int ans = 0;
	for (unsigned int i = 0; i < size - 1; ++i)
	{
		assert(MPI_Recv(buffers[i], 1, MPI_UNSIGNED, int(i + 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) == MPI_SUCCESS);
	  Printf("Get ans from %2u = %6u\n", i + 1, buffers[i][0]);
		ans += buffers[i][0];
	}

	printf("Answer: %u\n", ans);
	
	free(requests);
	for (unsigned int i = 0; i < size-1; ++i)
		free(buffers[i]);
	free(buffers);
	return;
}

void client_job(int size, int rank, unsigned int N)
{
	double start_time = MPI_Wtime();

	assert(size > 1);
	assert(rank > 0);
	assert(N > 1);
		
	// buffer for client
	unsigned int* buffer = (unsigned int*)calloc(2, sizeof(buffer[0]));
	assert(buffer);

	assert(MPI_Recv(buffer, 2, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) == MPI_SUCCESS);
	assert(buffer);
	assert(buffer[0] <= buffer[1]);

	Printf("%2u: process get information: from %6u to %6u\n", rank, buffer[0], buffer[1]);
	
	// client counts
	unsigned int ans = 0;
	for (unsigned int i = buffer[0]; i <= buffer[1]; ++i)
		ans += i;
	buffer[0] = ans;

	assert(MPI_Send(buffer, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD) == MPI_SUCCESS);

	Printf("%2d: process killed with ans: %u\n", rank, ans);
	printf("%2d: PROC TIME IN SEC: %lg\n", rank, MPI_Wtime() - start_time);	

	free(buffer);
	return;
}

// if size == 1
unsigned int sum(unsigned int N)
{
	assert(N > 0);

	unsigned ans = 0;
	for (unsigned int i = 1; i <= N; ++i)
		ans += i;

	return ans;
}


int main(int argc, char** argv)
{
  int size, rank;
	
	assert(argc == 2);
	unsigned int N = (unsigned int)atoi(argv[1]);
	assert(N > 0);

  assert(MPI_Init(NULL, NULL) == MPI_SUCCESS);
	double start_time = MPI_Wtime();	
	
	assert(MPI_Comm_size(MPI_COMM_WORLD, &size) == MPI_SUCCESS);
  assert(MPI_Comm_rank(MPI_COMM_WORLD, &rank) == MPI_SUCCESS);

	// special cases
	if (N == 1)
	{
		if (rank != 0)
		{
			assert(MPI_Finalize() == MPI_SUCCESS);
			return 0;
		}

		printf("Ans: %u\n", 1);
		assert(MPI_Finalize() == MPI_SUCCESS);
		return 0;
	}

	if (size == 1)
	{
		if (rank != 0)
			return 0;
		
		printf("Ans: %u\n", sum(N));
		assert(MPI_Finalize() == MPI_SUCCESS);
		return 0;
	}

	if (size >= N + 2)
	{
		size = int(N) + 1;
		if (rank > N)
		{
			assert(MPI_Finalize() == MPI_SUCCESS);
			return 0;
		}
	}
	
	// general case
	if (rank == 0)
  {
		server_job(size, rank, N);
	}
	else
	{
		client_job(size, rank, N);
		assert(MPI_Finalize() == MPI_SUCCESS);
	  return 0;	
	}
	
	printf("TOTAL TIME IN SEC: %lg\n", MPI_Wtime() - start_time);	
  assert(MPI_Finalize() == MPI_SUCCESS);
  return 0;
}
