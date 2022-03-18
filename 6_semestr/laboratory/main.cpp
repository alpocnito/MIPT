#include "math.h"
#include "mpi.h"
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"
#define MCW MPI_COMM_WORLD
#define MRF MPI_REQUEST_FREE

#define DEBUG

#ifdef DEBUG
	#define Printf(...) printf(__VA_ARGS__);
#else
	#define Printf(...) ;
#endif

const double X = 10;
const double T = 10;

void OneProc(unsigned K, unsigned M);
void SeveralProc(unsigned K, unsigned M, unsigned size, unsigned rank);
double f(double t, double x);
double phi(double x);
double psi(double t);
void print_u(double** u, unsigned K, unsigned M);
void print_line(double* u_i, unsigned K, unsigned M, unsigned i);


int main(int argc, char** argv)
{
	assert(argc == 3);
	unsigned K = (unsigned)atoi(argv[1]);
	unsigned M = (unsigned)atoi(argv[2]);
	assert(K > 0);
	assert(M > 0);
	

  assert(MPI_Init(NULL, NULL) == MPI_SUCCESS);
	int i_size, i_rank;
	assert(MPI_Comm_size(MCW, &i_size) == MPI_SUCCESS);
  assert(MPI_Comm_rank(MCW, &i_rank) == MPI_SUCCESS);
	assert(i_size >= 0 && i_rank >= 0);
	unsigned size = (unsigned)i_size;
	unsigned rank = (unsigned)i_rank;
	
	
	double start_time = MPI_Wtime();
	if (size <= 1)
	{
		OneProc(K, M);
  	assert(MPI_Finalize() == MPI_SUCCESS);
		return 0;
	}
	
	SeveralProc(K, M, size, rank);	

	if (rank == 0) printf("Time: %lg\n", MPI_Wtime() - start_time);
  assert(MPI_Finalize() == MPI_SUCCESS);
	return 0;
}

double f(double t, double x)
{
	return t * cos(x);
}

double phi(double x)
{
	return pow(x, 2);
}

double psi(double t)
{
	return 2 * sin(2*t);
}

void OneProc(unsigned K, unsigned M)
{
	assert(K > 0);
	assert(M > 0);
	
	double** u = (double**)calloc(K+1, sizeof(u[0]));
	double tau = T / K;
	double h   = X / M;
	
	for (unsigned i = 0; i <= K; ++i)
		u[i] = (double*)calloc(M+1, sizeof(u[0][0]));
	
	for (unsigned i = 0; i <= K; ++i)
		u[i][0] = psi(i * T / K);
	for (unsigned i = 0; i <= M; ++i)
		u[0][i] = phi(i * X / M);
	
	// print_u(u, K, M);
  
	for (unsigned k = 0; k < K; ++k)
	{
		for (unsigned m = 1; m <= M; ++m)
			u[k+1][m] = f(k * T/K, m * X/M) * tau + u[k][m] - (u[k][m] - u[k][m-1]) * tau / h;
	}
	// print_u(u, K, M);
	
	for (unsigned i = 0; i <= K; ++i)
		free(u[i]);
	free(u);
}


void SeveralProc(unsigned K, unsigned M, unsigned size, unsigned rank)
{
	assert(K > 0);
	assert(M > 0);
	assert(size > 1);
	
	if (rank == 0)
	{
		double* u_0 = (double*)calloc(M+1, sizeof(u_0[0])); 
		for (unsigned i = 0; i <= M; ++i)
		{
			u_0[i] = phi(i * X / M);
			assert(MPI_Isend(&u_0[i], 1, MPI_DOUBLE, rank+1, i, MCW) == MPI_SUCCESS);
		}
		//print_line(u_0, K, M, 0);
		free(u_0)
	}
	else
	{
		printf("1");		
	}
}


void print_line(double* u_i, unsigned K, unsigned M, unsigned i)
{
	assert(u_i);
	assert(K > 0);
	assert(M > 0);
	assert(i <= K);
}


void print_u(double** u, unsigned K, unsigned M)
{
	assert(u);
	assert(K > 0);
	assert(M > 0);
		
	printf("      ");
	for (unsigned i = 0; i <= M+1; ++i)
		printf("------");
	
	printf("\nK     |M    ");
	for (int i = 0; i <= M; ++i)
		printf("%-6u", i);

	for (int i = 0; i <= int(K); ++i)
	{
		printf("\n%-6u", i);
		printf("|     ");
		
		for (unsigned j = 0; j <= M; ++j)
			printf("%-6.1lg", u[i][j]);
		
		printf("\n      ");
	}
	printf("\n\n\n");
}
 


