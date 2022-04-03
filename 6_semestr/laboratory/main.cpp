#include "math.h"
#include "mpi.h"
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"
#include "string.h"
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

void* Calloc(size_t nmemb, size_t size)
{
    void* ptr = calloc(nmemb, size);
    
    if (ptr == NULL)
    {
        perror("ERROR\n\nCalloced size is too big\n\n\n");
        assert(ptr);
    }
    return ptr;
}


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
	
	double** u = (double**)Calloc(K+1, sizeof(u[0]));
	double tau = T / K;
	double h   = X / M;
	
	for (unsigned i = 0; i <= K; ++i)
		u[i] = (double*)Calloc(M+1, sizeof(u[0][0]));
	
	for (unsigned i = 0; i <= K; ++i)
		u[i][0] = psi(i * T / K);
    for (unsigned i = 0; i <= M; ++i)
		u[0][i] = phi(i * X / M);
	
  
	for (unsigned k = 0; k < K; ++k)
	{
		for (unsigned m = 1; m <= M; ++m)
        {
            u[k+1][m] = f(k * T/K, m * X/M) * tau + u[k][m] - (u[k][m] - u[k][m-1]) * tau / h;
            //printf("(%u, %u)   F: %lg; u_1: %lg; u_2: %lg\n", k+1, m, f(k * T/K, m * X/M), u[k][m], u[k][m-1]);
	    }
    }
	//Print_u(u, K, M);
	
	for (unsigned i = 0; i <= K; ++i)
		free(u[i]);
	free(u);
}

void Print_several_proc(unsigned K, unsigned M, unsigned size, unsigned rank, double* u, unsigned width)
{   

    unsigned all_width = (M + 1) / size;
    unsigned last_width = width;

    if (rank != size-1)
    {
        MPI_Request req;
        assert(MPI_Isend(u, int(all_width * (K + 1)), MPI_DOUBLE, int(size-1), int(rank), MCW, &req) == MPI_SUCCESS);
    }
        
    else
    {  
        // u with K * M elements
        double** u_all = (double**) Calloc((K + 1), sizeof(u_all[0]));
        for (unsigned i = 0; i <= K; ++i)
            u_all[i] = (double*) Calloc((M + 1), sizeof(u_all[0][0]));
        
        // Received u from smaller rank processes
        double** u_recv = (double**) Calloc(size, sizeof(u_recv[0]));
        for (unsigned i = 0; i < size-1; ++i)
        {
            u_recv[i] = (double*) Calloc(all_width * (K + 1), sizeof(u_recv[0][0]));
		    //printf("TEST\n"); 
            assert(MPI_Recv(u_recv[i], int(all_width * (K + 1)), MPI_DOUBLE, MPI_ANY_SOURCE, int(i), MCW, MPI_STATUS_IGNORE) == MPI_SUCCESS);
		    //printf("TEST\n"); 
        } 
        
        // u from rank == size-1 process
        u_recv[size-1] = (double*) Calloc(last_width*(K + 1), sizeof(u_recv[0][0]));
        assert(memcpy(u_recv[size-1], u, sizeof(double) * last_width * (K + 1)));
 
        /*
        for (unsigned j = 0; j < all_width * (K+1); ++j)
            printf("%lg ", u_recv[0][j]); 
        printf("\nwidth: %u\n", all_width);
        
        for (unsigned j = 0; j < last_width * (K+1); ++j)
            printf("%lg ", u_recv[1][j]); 
        printf("\nwidth: %u\n", last_width);
        */

        // filling K*M u
        for (unsigned i = 0; i <= K; ++i)
        {   
            unsigned last_u_pos = 0;
            for (unsigned j = 0; j < size; ++j)
            {
                unsigned t_width = (j == size-1) ? last_width : all_width;

                for (unsigned k = 0; k < t_width; ++k)
                {
                    u_all[i][last_u_pos + k] = u_recv[j][k + i*t_width];
                }
                last_u_pos += t_width;
            }
        }

        print_u(u_all, K, M);

        for (unsigned i = 0; i <= K; ++i)
           free(u_all[i]); 
         free(u_all);
        
        for (unsigned i = 0; i < size; ++i)
            free(u_recv[i]);    
        free(u_recv); 

    }
}
void SeveralProc(unsigned K, unsigned M, unsigned size, unsigned rank)
{
	assert(K > 0);
	assert(M > 0);
	assert(size > 1);
	
	if (rank >= M)
		return;
    
    if (size > M + 1)
        size = M + 1;

	double tau = T / K;
	double h   = X / M;

    // Algorithm will calculate on [first_col; last_col)
    unsigned div = (M + 1) / size;

    unsigned first_col = (rank)     * div;
    unsigned last_col  = (rank + 1) * div;
    if (rank == size - 1) last_col = M + 1;
    
    unsigned width = last_col - first_col;
    assert(width > 0);
    //printf("rank : %u; width: %u\n", rank, width);

    double* u = (double*) Calloc(width * (K + 1), sizeof(u[0]));
    

    // fill first row
    for (unsigned i = 0; i < width; ++i)
        u[i] = phi( (i + first_col) * X / M );
    
    for (unsigned i = 1; i <= K; ++i)
    {
        // Calculate last element ans send it
	    if (width > 1)
        {
            u[i*width + width - 1] = f((i-1) * T/K, (last_col - 1)  * X/M) * tau + 
            u[(i-1)*width + width - 1] - 
            (u[(i-1)*width + width - 1] - u[(i-1)*width + width - 2]) * tau / h;
	        //printf("% u; LAST: %lg\n", rank, u[i*width + width - 1]);	
        }

        if (rank != size - 1)
        {
            MPI_Request req;
            assert(MPI_Isend(&(u[i*width + width - 1]), 1, MPI_DOUBLE, 
                        int(rank + 1), int(i+1), MCW, &req) == MPI_SUCCESS);
        }

        // Calculate other elements
        if (width > 2)
        {
            for (unsigned j = 1; j < width - 1; ++j)
                u[i * width + j] = f((i-1) * T/K, (first_col + j) * X/M ) * tau + 
                u[(i-1) * width + j] - 
                (u[(i-1) * width + j] - u[(i-1) * width + j-1]) * tau / h ;
        }

        // Calculate first element
        if (rank == 0)
        {
            u[i*width] = psi(i *T/K);
        }
        else if (i == 1)
        {
            double recv_u = phi( (first_col-1) * X / M );
    
            u[i*width] = f((i-1) * T/K, first_col * X/M) * tau + u[(i-1)*width] - 
            (u[(i-1)*width] - recv_u) * tau / h;
        }
        else
        {
            double recv_u = 0;
            assert(MPI_Recv(&recv_u, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 
                        int(i), MCW, MPI_STATUS_IGNORE) == MPI_SUCCESS);
	        

            u[i*width] = f((i-1) * T/K, first_col * X/M) * tau + u[(i-1)*width] - 
            (u[(i-1)*width] - recv_u) * tau / h;
            //printf("(%u, %u)   F: %lg; u_1: %lg, u_2: %lg\n", i, first_col, f((i-1) * T/K, first_col * X/M), u[(i-1)*width], recv_u);
        }
    }
    

    //for (unsigned i = 0; i < width * (K+1); ++i)
    //    printf("%lg ", u[i]);
    //printf("\n---\n");
    
    //Print_several_proc(K, M, size, rank, u, width);
    free(u);
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
