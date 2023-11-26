/*
Duas tasks imprimem quando associadas a até duas threads

As saídas abaixo são possíveis:
A race car 
A car race

"A" sempre vem antes.
*/


#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


int main(int argc, char *argv[]) 
{
    #pragma omp parallel num_threads(2)
    {
	#pragma omp single
	{
	    printf("A ");
	    
	    #pragma omp task
	    {printf("race ");}
	    
	    #pragma omp task
	    {printf("car ");}
	    
//	    printf("is fun to watch ");
	}

    } // End of parallel region

    printf("\n");
    return(0);

}
