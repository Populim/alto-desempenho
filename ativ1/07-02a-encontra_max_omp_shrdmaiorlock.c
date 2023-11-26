// to compile: gcc 07-02a-encontra_max_omp_shrdmaiorlock.c -o 07-02a-encontra_max_omp_shrdmaiorlock -fopenmp
// to execute: 07-02a-encontra_max_omp_shrdmaiorlock <num_elements>
//
// regiao critica esta dentro do loop.
//

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define T 8

int main(int argc,char **argv){
    double wtime;
    
    int *vetor, i, maior, localmaior, tam, num_threads;
	
	if ( argc  != 2)
	{
	    printf("Wrong arguments. Please use main <amount_of_elements>\n");
	    exit(0);
	} // fim do if

	tam = atoi(argv[1]);

	printf("Amount of vetor=%d\n", tam);
	fflush(0);

	vetor=(int*)malloc(tam*sizeof(int)); //Aloca o vetor da dimensão lida

	// iniciando vetor e fixando o maior valor para validacao
	for(i = 0; i < tam; i++)
	{
	    vetor[i] = 1;
	} // fim do for
	vetor[tam/2] = tam;
      
	omp_lock_t mylock;
	omp_init_lock(&mylock);

    wtime = omp_get_wtime();

    localmaior = maior = -1;
    #pragma omp parallel num_threads(T) shared (maior) private(localmaior)
    {
		int my_id;
		
		// determina o nr da thread
		my_id = omp_get_thread_num();  //

		// determina o nr de threads
		num_threads = omp_get_num_threads();

		#pragma omp for private(i)
		for(i = 0; i < tam; i++)
		{
            omp_set_lock(&mylock);
                if(vetor[i] > maior)
                    maior=vetor[i];
            omp_unset_lock(&mylock);
                
        } // fim do for
	} // fim da regiao paralela

   /*
    *************************************************************************************
    Não modifique este trecho de código
    */
    wtime = omp_get_wtime() - wtime;

    printf("OMP SHD LOCK: Tam=%d, Num_Threads=%d, maior=%d, Elapsed wall clock time = %f  \n", tam, num_threads, maior, wtime); //Imprime o vetor ordenado
    free(vetor); //Desaloca o vetor lido
    return 0;
} // fim da main
