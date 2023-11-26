// compilar: gcc 07-04-encontra_max_omp_reduc.c -o a -fopenmp
// executar: a <num_elements>
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
	int *vetor, tam;
	int maior[T], the_greatest;
	int i,j;
    int my_id;
    int aux;
    if ( argc  != 2)
    {
		printf("Wrong arguments. Please use binary <amount_of_elements>\n");
		exit(0);
    } // fim do if

    tam = atoi(argv[1]);

    printf("Amount of vetor=%d\n", tam);
    fflush(0);
	
	vetor=(int*)malloc(tam*sizeof(int)); //Aloca o vetor da dimensão lida

    /************************************************************************************
		Modifique a partir daqui
    */
	

	// iniciando vetor e fixando o maior valor para validacao
	
	wtime = omp_get_wtime();

	for(i = 0; i < tam; i++)
		vetor[i] = 1;

	wtime = omp_get_wtime() - wtime;

	printf("atribuição sequencial: %f\n", wtime);

	wtime = omp_get_wtime();

	#pragma omp parallel num_threads(T) private(j,i,my_id)
	{
		my_id = omp_get_thread_num();
		for (j = 0; j < T; j++){
			if(my_id == j){
				for(i = (tam/T)*(my_id); i < (tam/T)*(my_id+1); i++){
					vetor[i] = 1;
				}
			}
		}
	}
//	#pragma omp barrier
	wtime = omp_get_wtime() - wtime;
	
	printf("atribuição paralela: %f\n", wtime);

	vetor[tam/2] = tam;

    wtime = omp_get_wtime();



//	____________|_____________________|_____________________|___________________    

	//maior = vetor[0];
	for (int i = 0; i < T; i++){
		maior[i] = vetor[0];
	}


	#pragma omp parallel num_threads(T) private(j,i,my_id,aux)
	{
		my_id = omp_get_thread_num();
		for (j = 0; j < T; j++){
			
			if(my_id == j){
				aux = (tam/T)*(my_id+1);
				for(i = (tam/T)*(my_id); i < aux; i++){
					//printf("thread %d: %d\n",my_id, i);
					if(vetor[i] > maior[0])
						maior[0] = vetor[i];
				}//_
			}
		}
	}
	
	the_greatest = maior[0];
	for (i = 1; i < T; i++){
		if(the_greatest < maior[i]){
			the_greatest = maior[i];
		}
	}

	//printf("%d %d %d %d\n", maior[0], maior[1], maior[2], maior[3]);

	/*
	*************************************************************************************
	Não modifique este trecho de código
	*/
   
	wtime = omp_get_wtime() - wtime;

    printf("PAR REDUCTION: Tam=%d, maior=%d, Elapsed wall clock time = %f  \n", tam, the_greatest, wtime); //Imprime o vetor ordenado
    free(vetor); //Desaloca o vetor lido
	
    return 0;
}
