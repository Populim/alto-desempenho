/*
Duas tasks imprimem quando associadas a até duas threads.

As saídas abaixo são possíveis:
01 - A is fun to watch race car
02 - A is fun to watch car race
03 - A race is fun to watch car
04 - A car is fun to watch race
05 - A car race is fun to watch
06 - A race car is fun to watch

"A" sempre vem antes.
"is fun to watch " pode vir antes, intercalado a, ou depois das strings "race" e "car"

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
	    
	    printf("is fun to watch ");	    
	}

    } // End of parallel region

    printf("\n");
    return(0);

}
