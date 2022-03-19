#include "math.h"
#include "mpi.h"
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"
#define MCW MPI_COMM_WORLD
#define MRF MPI_REQUEST_FREE

int main(int argc, char** argv)
{
  assert(MPI_Init(NULL, NULL) == MPI_SUCCESS);
	int i_size, i_rank;
	assert(MPI_Comm_size(MCW, &i_size) == MPI_SUCCESS);
  assert(MPI_Comm_rank(MCW, &i_rank) == MPI_SUCCESS);
	assert(i_size >= 0 && i_rank >= 0);
	unsigned size = (unsigned)i_size;
	unsigned rank = (unsigned)i_rank;
		
	if (rank == 0)
	{
		double start_time = MPI_Wtime();
		MPI_Send(&start_time, 1, MPI_DOUBLE, 1, 0, MCW);
		assert(MPI_Finalize() == MPI_SUCCESS);
		return 0;
	}
	else
	{
		double start_time = 0;
		MPI_Recv(&start_time, 1, MPI_DOUBLE, 0, 0, MCW, MPI_STATUS_IGNORE);
		printf("Time: %lg\n", MPI_Wtime() - start_time);
		assert(MPI_Finalize() == MPI_SUCCESS);
		return 0;
	}
}
