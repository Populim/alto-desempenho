/*
Duas tasks imprimem quando associadas a até duas threads.

As saídas abaixo são possíveis:
A car race is fun to watch
A race car is fun to watch

"A" sempre vem antes.
"is fun to watch " vem depois das strings "race" e "car"

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
	    
	    #pragma omp taskwait  // wait child (previous) tasks complete
	    printf("is fun to watch ");	    
	}

    } // End of parallel region

    printf("\n");
    return(0);

}