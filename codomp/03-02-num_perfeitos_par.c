//to compile: gcc 03-02-num_perfeitos_par.c -o 03-02-num_perfeitos_par -fopenmp
// to run: 03-02-num_perfeitos_par <amount_of_items>
//
// Expected outuput for main 10
//6 é perfeito. 
//
// Expected outuput for main 1000
//6 é perfeito. 
//28 é perfeito. 
//496 é perfeito.
//
// Expected output for main 10000
//6 é perfeito. 
//28 é perfeito. 
//496 é perfeito. 
//8128 é perfeito. 
//
//// Expected output for main 100000
//6 é perfeito. 
//28 é perfeito. 
//496 é perfeito. 
//8128 é perfeito. 
//

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define T 8

int main(int argc, char *argv[])
{
    int i, j, num, soma;

    double wtime;

    
    if ( argc  != 2)
    {
	printf("Wrong arguments. Please use main <amount_of_elements>\n");
	exit(0);
    }

    num = atoi(argv[1]);
    
    wtime = omp_get_wtime();

    #pragma omp parallel for private (i, j, soma) num_threads(T) /*schedule(guided, 250)*/   /*schedule(dynamic, 1)*/
    for (i = 1; i < num; i++)
    {
        soma = 0;
        for (j = 1; j < i; j++)
        {
            if(i%j == 0)
            {
                soma += j;
            }
        }
        if (soma == i)
        {
            printf("%d é perfeito. \n", i);
        }
    }

    wtime = omp_get_wtime() - wtime;

    printf ( "Done. Elapsed wall clock time = %.5f\n", wtime );

    return 0;
}
