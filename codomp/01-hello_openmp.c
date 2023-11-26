// compile: gcc 01-hello_openmp.c -o 01-hello_openmp -lm -fopenmp
//
// run: 01-hello_openmp
//
// source code from: https://people.sc.fsu.edu/~jburkardt/c_src/openmp/openmp.html
//

# include <stdlib.h>
# include <stdio.h>
# include <omp.h>

#define T 4

int main ( int argc, char *argv[] )
{  
	int thread_num, nthreads, soma = 0;  

	thread_num = omp_get_thread_num ( );  
	nthreads = omp_get_num_threads();  
	printf ( "Hello-world da thread mestre  (região sequencial): %d. Num_threads=%d. \n", thread_num, nthreads ) ; 

	# pragma omp parallel private (thread_num, nthreads) num_threads(T) reduction(+: soma)
	{    
		thread_num = omp_get_thread_num();    
		nthreads = omp_get_num_threads( );
		soma = thread_num;
		printf (" \nHello-world da thread %d na região paralela, Num_threads aqui: %d\n", thread_num, nthreads);
	}  

	thread_num = omp_get_thread_num();
	nthreads = omp_get_num_threads();
	printf ( "\nRegião sequencial: thread_num = %d, Num_threads = %d e soma=%d \n", thread_num, nthreads, soma);
	return 0;
}
