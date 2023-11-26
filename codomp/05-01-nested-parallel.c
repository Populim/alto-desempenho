//to compile: gcc 05-01-nested-parallel.c -o 05-01-nested-parallel -fopenmp
// to run: 05-01-nested-parallel


#include <stdio.h>
#include <omp.h>

#define NT1 2
#define NT2 4

int main()
{
    omp_set_nested(0);

    #pragma omp parallel num_threads(NT1)    
    {
        int tid01, in_par01, num_th01;
	
		tid01 = omp_get_thread_num();
		num_th01 = omp_get_num_threads( );
		in_par01 = omp_in_parallel( );
	
		printf("Thread 1st level=>tid01:%d, in_par01:%d, num_th01:%d \n", tid01, in_par01, num_th01);
		fflush(0);
	
		#pragma omp parallel num_threads(NT2) firstprivate(tid01)
		{
			int tid02, in_par02, num_th02;
   
			tid02 = omp_get_thread_num();
			num_th02 = omp_get_num_threads( );
			in_par02 = omp_in_parallel( );
	
			printf("----------Thread 2nd level=>tid01:%d, tid02:%d, in_par02:%d, num_th02:%d \n", tid01, tid02, in_par02, num_th02);
			fflush(0);
  
		} // end of parallel 02
    } // end of parallel 01
} // end of main
