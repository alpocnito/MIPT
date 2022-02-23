#include "mpi.h"
#include "stdio.h"
#include "assert.h"

int main()
{
  int size, rank;

  // Инициация MPI API
  assert(MPI_Init(NULL, NULL) == MPI_SUCCESS);
  // Количество процессоров
  assert(MPI_Comm_size(MPI_COMM_WORLD, &size) == MPI_SUCCESS);
  // Номер текущего процессора
  assert(MPI_Comm_rank(MPI_COMM_WORLD, &rank) == MPI_SUCCESS);
	
	assert(size > 0);
	assert(rank >= 0);

  printf("Hello, %d of %d\n", rank, size);
  
  // Деинициалиция MPI API
  assert(MPI_Finalize() == MPI_SUCCESS);
  return 0;
}
