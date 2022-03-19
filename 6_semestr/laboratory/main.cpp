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

#define PRINT

#ifdef PRINT
	#define Print_line(...) print_line(__VA_ARGS__);
	#define Print_u(...) print_u(__VA_ARGS__);
#else
	#define Print_line(...) ;
	#define Print_u(...) ;
#endif


const double X = 10;
const double T = 10;

void OneProc(unsigned K, unsigned M);
void SeveralProc(unsigned K, unsigned M, unsigned size, unsigned rank);
double f(double t, double x);
double phi(double x);
double psi(double t);
void print_u(double** u, unsigned K, unsigned M);
void print_line(double* u_i, unsigned K, unsigned M, int i);


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
		printf("Time: %lg\n", MPI_Wtime() - start_time);

  	assert(MPI_Finalize() == MPI_SUCCESS);
		return 0;
	}
	
	SeveralProc(K, M, size, rank);	

	if (rank == K % size) 
		printf("Time: %lg\n", MPI_Wtime() - start_time);
  
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
	
  
	for (unsigned k = 0; k < K; ++k)
	{
		for (unsigned m = 1; m <= M; ++m)
			u[k+1][m] = f(k * T/K, m * X/M) * tau + u[k][m] - (u[k][m] - u[k][m-1]) * tau / h;
	}
	Print_u(u, K, M);
	
	for (unsigned i = 0; i <= K; ++i)
		free(u[i]);
	free(u);
}


void SeveralProc(unsigned K, unsigned M, unsigned size, unsigned rank)
{
	assert(K > 0);
	assert(M > 0);
	assert(size > 1);
	
	if (rank > K)
		return;

	double tau = T / K;
	double h   = X / M;
	
	if (rank == 0)
	{
		double* u_0 = (double*)calloc(M+1, sizeof(u_0[0])); 
		for (unsigned i = 0; i <= M; ++i)
		{
			u_0[i] = phi(i * X / M);
			MPI_Request req;
			assert(MPI_Isend(&u_0[i], 1, MPI_DOUBLE, int( (rank+1) % size ), int(i), MCW, &req) == MPI_SUCCESS);
		}
		
		Print_line(u_0, K, M, -1);
		Print_line(u_0, K, M, int(rank));
		free(u_0);
	
		SeveralProc(K, M, size, rank + size);
	}
	else
	{
		double* u_i = (double*)calloc(M+1, sizeof(u_i[0]));
		u_i[0] = psi(rank * T / K);

		double u_p1 = 0;
		double u_p2 = 0;
		assert(MPI_Recv(&u_p1, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MCW, MPI_STATUS_IGNORE) == MPI_SUCCESS);
		
		MPI_Request req;
		assert(MPI_Isend(&u_i[0], 1, MPI_DOUBLE, int( (rank+1) % size ), 0, MCW, &req) == MPI_SUCCESS);

		for (unsigned j = 1; j <= M; ++j)
		{
			assert(MPI_Recv(&u_p2, 1, MPI_DOUBLE, MPI_ANY_SOURCE, int(j), MCW, MPI_STATUS_IGNORE) == MPI_SUCCESS);
			
			u_i[j] = f((rank-1) * T/K, j * X/M) * tau + u_p2 - (u_p2 - u_p1) * tau / h;
			
			assert(MPI_Isend(&u_i[j], 1, MPI_DOUBLE, int( (rank+1) % size ), int(j), MCW, &req) == MPI_SUCCESS);

			u_p1 = u_p2;
		}

		Print_line(u_i, K, M, int(rank));
		free(u_i);

		SeveralProc(K, M, size, rank + size);
	}
}


void print_line(double* u_i, unsigned K, unsigned M, int i)
{
	assert(u_i);
	assert(K > 0);
	assert(M > 0);
	assert(i <= int(K));

	if (i == -1)
	{
		printf("\n            ");
		for (int j = 0; j <= M; ++j)
			printf("%-6u", j);
		
		printf("\n      ");
		for (unsigned j = 0; j <= M+1; ++j)
			printf("------");
	
	}
	else if (i >= 0)
	{
		printf("\n%-6u", i);
		printf("|     ");
		
		for (unsigned j = 0; j <= M; ++j)
			printf("%-6.1lg", u_i[j]);
		
		printf("\n      |");
	
		if (i == K)
			printf("\n\n\n");
	}
	else
	{
		printf("Unknown i: %d\n", i);
	}
}


void print_u(double** u, unsigned K, unsigned M)
{
	assert(u);
	assert(K > 0);
	assert(M > 0);
	
	printf("\n            ");
	for (int i = 0; i <= M; ++i)
		printf("%-6u", i);

	
	printf("\n      ");
	for (unsigned i = 0; i <= M+1; ++i)
		printf("------");
	
	for (int i = 0; i <= int(K); ++i)
	{
		printf("\n%-6u", i);
		printf("|     ");
		
		for (unsigned j = 0; j <= M; ++j)
			printf("%-6.1lg", u[i][j]);
		
		printf("\n      |");
	}
	printf("\n\n\n");
}
 


