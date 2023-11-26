// to compile: gcc 02-01a-soma_elementos_vetor_seq_args.c -o 02-01a-soma_elementos_vetor_seq_args -fopenmp
// to run: 02-01a-soma_elementos_vetor_seq_args <amount_of_items> <seed>
//
// Expected outuput for main 10 10
//61
//
// Expected outuput for main 5 2
//23
//
// Expected output for main 100000000 1 
//450029111


#include<stdio.h>
#include<stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]){
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
        

    wtime = omp_get_wtime();

    //Faz a soma dos elementos
    for(i=0;i<tam;i++)
        soma+= (long int) vetor[i];

    
    wtime = omp_get_wtime() - wtime;

    
    printf ("Elapsed wall clock time = %.5f\n", wtime );
    printf("%ld\n",soma);
    free(vetor);
    
    return(0);

}
