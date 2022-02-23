//
// Сделаны все доп. задания
//

#include "mpi.h"
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"

#define DEBUG

#ifdef DEBUG
	#define Printf(...) printf(__VA_ARGS__);
#else
	#define Printf(...) ;
#endif

void star(int size, int rank, int* arr);
void ring(int size, int rank, int* arr);

int main(int argc, char** argv)
{
	assert(argc >= 3);
	
	// read array from command prompt
	int* arr = (int*)calloc(size_t(argc-2), sizeof(arr[0]));
	assert(arr);

	for (unsigned i = 0; i < argc - 2; ++i)
		arr[i] = atoi(argv[i+2]);

  assert(MPI_Init(NULL, NULL) == MPI_SUCCESS);
  int size, rank;
	assert(MPI_Comm_size(MPI_COMM_WORLD, &size) == MPI_SUCCESS);
  assert(MPI_Comm_rank(MPI_COMM_WORLD, &rank) == MPI_SUCCESS);
	assert(size == argc-2);	
	

	// Topology star
	if (argv[1][0] == 's')
		star(size, rank, arr);
	
	// Topology ring
	if (argv[1][0] == 'r')
		ring(size, rank, arr);
	
  assert(MPI_Finalize() == MPI_SUCCESS);
	return 0;
}

void ring(int size, int rank, int* arr)
{
	assert(size >= 0);
	assert(rank >= 0);
	assert(arr);
	
	if (rank != 0)
		assert(MPI_Recv(arr, size, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) == MPI_SUCCESS);


	printf("%2d: Get data = %2d\n", rank, arr[rank]); 


	if (rank != size-1)
		assert(MPI_Send(arr, size, MPI_INT, rank+1, 0, MPI_COMM_WORLD) == MPI_SUCCESS);

	return;
}

void star(int size, int rank, int* arr)
{
	assert(size >= 0);
	assert(rank >= 0);
	assert(arr);
	
	// Hub give array to all clients
	if (rank == 0)
	{
		printf("Hub get = %d\n", arr[0]);
		for (int i = 1; i < size; ++i)
		{
			assert(MPI_Send(arr, size, MPI_INT, i, 0, MPI_COMM_WORLD) == MPI_SUCCESS);
			assert(MPI_Recv(arr, size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) == MPI_SUCCESS);
		}
	}

	// Cliens print their value
	else
	{
		assert(MPI_Recv(arr, size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) == MPI_SUCCESS);
		printf("%2d: Get data = %2d\n", rank, arr[rank]); 
		assert(MPI_Send(arr, size, MPI_INT, 0, 0, MPI_COMM_WORLD) == MPI_SUCCESS);
	}

  return;
}
