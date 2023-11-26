//to compile: gcc 05-02-nested-section.c -o 05-02-nested-section -fopenmp
// to run: 05-02-nested-section

#include <stdio.h>
#include <omp.h>

#define NT1 4
#define NT2 2

void doTask(int tasknr, int tid01) 
{
    int i, tid02, in_par02, num_th02;

    printf("  doTask:%d, tid01:%d \n", tasknr, tid01);
    fflush(0);
    
    #pragma omp parallel num_threads(NT2) private(i, tid02, in_par02, num_th02)
    {
		tid02 = omp_get_thread_num();
		num_th02 = omp_get_num_threads( );
		in_par02 = omp_in_parallel( );
	
		#pragma omp for private(i)
        for(i=0; i<NT2; i++) 
		{
			printf("-------- Task%d, thread 2nd level tid01:%d, tid02:%d, in_par02:%d, num_threads02:%d \n", tasknr, tid01, tid02, in_par02, num_th02);
			fflush(0);
		}
    }
}

int main()
{
    int tid01, in_par01, num_th01;
    
    omp_set_nested(1);

    #pragma omp parallel num_threads(NT1) private(tid01, in_par01, num_th01)
    {
		tid01 = omp_get_thread_num();
		in_par01 = omp_in_parallel( );
		num_th01 = omp_get_num_threads( );

		printf("Thread 1st level==> tid01:%d, in_par01:%d, num_threads01:%d \n", tid01, in_par01, num_th01);
		fflush(0);
		#pragma omp sections 
		{
			#pragma omp section
			{
				printf("---main section 1 omp_get_thread_num():%d \n", omp_get_thread_num());
				fflush(0);
				doTask(1, tid01);
			}
			#pragma omp section
			{
				printf("---main section 2 omp_get_thread_num():%d \n", omp_get_thread_num());
				fflush(0);
				doTask(2, tid01);
			}
			#pragma omp section
			{
				printf("---main section 3 omp_get_thread_num():%d \n", omp_get_thread_num());
				fflush(0);
				doTask(3, tid01);
			}
			#pragma omp section
			{
				printf("---main section 4 omp_get_thread_num():%d \n", omp_get_thread_num());
				fflush(0);
				doTask(4, tid01);
			}
		} // end sections
	} // end parallel
} // end main
