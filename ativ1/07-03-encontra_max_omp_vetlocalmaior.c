// compilar: gcc 07-03-encontra_max_omp_vetlocalmaior.c -o 07-03-encontra_max_omp_vetlocalmaior -fopenmp
// executar: 07-03-encontra_max_omp_vetlocalmaior <num_elements>
//

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define T 8

int main(int argc,char **argv){
    double wtime;

    /*
    Não modifique este trecho de código
    *************************************************************************************
    */
    int *vetor, i, tam, num_threads;
	int maior, local_maior[T]; // havera um maior global e um vetor para o maior local para cada thread
		
    if ( argc  != 2)
    {
		printf("Wrong arguments. Please use binary <amount_of_elements>\n");
		exit(0);
    } // fim do if

    tam = atoi(argv[1]);

    printf("Amount of vetor=%d\n", tam);
    fflush(0);

	vetor=(int*)malloc(tam*sizeof(int)); //Aloca o vetor da dimensão lida
//    for(i=0;i<tam;i++)
//        fscanf(stdin, "%d\n", &(vetor[i])); //Lê os elementos do vetor
//    

    // iniciando vetor com nrs aleatorios e fixando o maior valor para validacao
    for(i = 0; i < tam; i++)
		vetor[i] = 1;

    vetor[tam/2] = tam;
    
    // iniciando vetor maiores locais com 0 
    for(i = 0; i < T; i++)
		local_maior[i] = 0;


    wtime = omp_get_wtime();

    #pragma omp parallel num_threads(T) shared (local_maior)
    {
		int my_id;

		// determina o nr de threads
		num_threads = omp_get_num_threads();
		
		// determina o nr da thread
		my_id = omp_get_thread_num();
		
		#pragma omp for private(i)
		for(i = 1; i < tam; i++)
		{
			if(vetor[i] > local_maior[my_id])
			  local_maior[my_id] = vetor[i];
		} // fim do for
		
    } // fim da regiao paralela

	maior = local_maior[0];
	for(i = 1; i < T; i++)
	{
		if (local_maior[i] > maior)
        {
			maior = local_maior[i];
        }
	} // fim do for
	
   /*
    *************************************************************************************
    Não modifique este trecho de código
    */

    wtime = omp_get_wtime() - wtime;

    printf("OMP Vet Maior Local: Tam=%d, Num_Threads=%d, maior=%d, Elapsed wall clock time = %f \n", tam, num_threads, maior, wtime); //Imprime o vetor ordenado
    free(vetor); //Desaloca o vetor lido
    return 0;
}
