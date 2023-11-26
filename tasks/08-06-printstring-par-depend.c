/*

*/

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


int main(int argc, char *argv[]) 
{
    int x=0;
    #pragma omp parallel num_threads(2)
    {
	#pragma omp single
	{
	    printf("A ");
	    
	    #pragma omp task depend(out: x) // depende de tarefas previas que têm "x" em in, out ou inout
	    {
	      printf("race ");	      
	    }
	    
	    #pragma omp task depend(in: x) // depende de tarefas previas que têm "x" em out ou inout
	    {
	      printf("car ");
	    }
	    
//	    printf("is fun to watch ");
	}

    } // End of parallel region

    printf("\n");
    return(0);

}