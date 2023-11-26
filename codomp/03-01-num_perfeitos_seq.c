//to compile: gcc 03-01-num_perfeitos_seq.c -o 03-01-num_perfeitos_seq -fopenmp
// to run: 03-01-num_perfeitos_seq <amount_of_items>
//
// Expected outuput for main 10
//6 é perfeito. 
//
// Expected outuput for main 1000
//6 é perfeito. 
//28 é perfeito. 
//496 é perfeito.
//
// Expected output for main main 10000
//6 é perfeito. 
//28 é perfeito. 
//496 é perfeito. 
//8128 é perfeito. 
//
//// Expected output for main main 100000
//6 é perfeito. 
//28 é perfeito. 
//496 é perfeito. 
//8128 é perfeito. 
//



#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

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
    
    for (i = 1; i < num; i++){
        soma = 0;
        for (j = 1; j < i; j++){
            if(i%j == 0){
                soma += j;
            }
        }
        if (soma == i){
            printf("%d é perfeito. \n", i);
        }
    }
    
    wtime = omp_get_wtime() - wtime;

    
    printf ( "Done. Elapsed wall clock time = %.5f\n", wtime );



    return 0;
}
