/*
Há duas threads executando e imprimindo as ambas as strings em paralelo.

As saídas abaixo são possíveis:
race car race car   (é o usual)
race race car car   (é possível - aqui tem variacoes das threads que imprimem e que produzem a mesma saida)

Replica as saídas. Não garante a ordem.
*/

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) 
{
    #pragma omp parallel num_threads(2)
    {
      printf("race ");
      printf("car ");
      
    } // End of parallel region
    
    printf("\n");

    return(0);

}
