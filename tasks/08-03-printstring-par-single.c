/*
Só uma thread imprime devido à diretiva single  

A saída abaixo é a única possível, mas só uma thread executa:
race car 

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
      printf("race ");
      printf("car ");
    }

  } // End of parallel region

  printf("\n");
  return(0);

}
