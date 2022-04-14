//
// Из доп. заданий посчитано число пи
//

#include "math.h"
#include "mpi.h"
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"
#define MCW MPI_COMM_WORLD
#define MU MPI_UNSIGNED

//#define DEBUG

#ifdef DEBUG
	#define Printf(...) printf(__VA_ARGS__);
#else
	#define Printf(...) ;
#endif

double job(unsigned start, unsigned end, char type);
double pi(unsigned index);
double exp(unsigned index);
unsigned fact(unsigned x);
double inv_fact(unsigned x);

int main(int argc, char** argv)
{
	assert(argc == 3);
	unsigned num_elems = (unsigned)atoi(argv[1]);
	char type = argv[2][0];

  assert(MPI_Init(NULL, NULL) == MPI_SUCCESS);
	int i_size, i_rank;
	assert(MPI_Comm_size(MCW, &i_size) == MPI_SUCCESS);
  assert(MPI_Comm_rank(MCW, &i_rank) == MPI_SUCCESS);
	assert(i_size >= 0 && i_rank >= 0);
	unsigned size = (unsigned)i_size;
	unsigned rank = (unsigned)i_rank;
	
	if (num_elems == 0)
	{
		if (rank == 0) printf("Ans: 0\n");
 		assert(MPI_Finalize() == MPI_SUCCESS);
		return 0;
	}
	// set start and end for each process
	unsigned start;
	unsigned end;
	if (num_elems < size)
		start = end = rank;
	else
	{
	 start = (rank == 0)      ? 0           : num_elems/size * rank; 
	 end   = (rank == size-1) ? num_elems-1 : num_elems/size * (rank + 1) - 1;  
	}
	Printf("%2u, start = %2u, end = %2u\n", rank, start, end);
	
	// main calculations
	double ans = 0;
	if (rank + 1 <= num_elems)
		ans = job(start, end, type);
	double total = 0;

	assert(MPI_Reduce(&ans, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MCW) == MPI_SUCCESS);

	if (rank == 0) printf("Ans: %lg\n", total);
  assert(MPI_Finalize() == MPI_SUCCESS);
	
	return 0;
}

double job(unsigned start, unsigned end, char type)
{
	assert(start <= end);
	
	double ans = 0;
	double (*func)(unsigned) = pi;
	switch (type)
	{
		case 'e':
			func = exp;
			break;
		case 'p':
			func = pi;
			break;
		default:
			printf("Unknown type. By default program calculates PI\n");
			break;
	}
	for (unsigned i = start; i <= end; ++i)
		ans += func(i);

	return ans;
}

double exp(unsigned index)
{
    if (index == 0)
		return 1;
	
	double ans = 1;
	for (unsigned i = 2; i <= index; ++i)
		ans /= i;

	return ans;
}

double pi(unsigned index)
{
	return 4 * pow(-1, index) / (2 * index + 1);
}

unsigned fact(unsigned x)
{
	if (x == 0)
		return 1;
	
	unsigned ans = 1;
	for (unsigned i = 2; i <= x; ++i)
		ans *= i;

	return ans;
}
