//to compile: gcc 02-03a-soma_elementos_vetor_par_reduction_args.c -o 02-03a-soma_elementos_vetor_par_reduction_args -fopenmp
// to run: 02-03a-soma_elementos_vetor_par_reduction_args <amount_of_items> <seed>
//
// Expected outuput for main 10 10
//501
//
// Expected outuput for main 5 2
//333
//
// Expected output for main main 100000000 1
//655147465



#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main(int argc, char *argv[])
{
    int tam,par,*vetor,i, seed;
    long int soma=0;
    
    double wtime;

      
    	
    if ( argc  != 3)
    {
	printf("Wrong arguments. Please use main <amount_of_elements> <seed_for_rand> \n");
	exit(0);
    }

    tam = atoi(argv[1]);
    seed = atoi(argv[2]);
		
    vetor=(int*)malloc(tam*sizeof(int)); // Alloc memory to vetor

    srand(seed);	// setting starting point to rand();
    for(i=0;i<tam;i++)
        vetor[i] = rand() % 10; // generate numbers

    wtime = omp_get_wtime ( );
        
    #pragma omp parallel for num_threads(4) private(i) reduction(+:soma)
    for(i=0;i<tam;i++)
           soma += (long int) vetor[i];

    wtime = omp_get_wtime ( ) - wtime;

    
    printf ( "Elapsed wall clock time = %.5f\n", wtime );
    printf("%ld\n",soma);
    free(vetor);
    
    return(0);
}
