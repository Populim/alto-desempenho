/*
Duas tasks em paralelo imprimem. Elas podem ser atribuídas às duas threads ou apenas a uma.

As saídas abaixo são possíveis:
race car 
car race

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
