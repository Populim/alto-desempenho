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
	int maiores[T], maior;
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
	

	wtime = omp_get_wtime();

	// iniciando vetor e fixando o maiores valor para validacao
	#pragma omp parallel num_threads(T) private(j,i,my_id,aux)
	{
		my_id = omp_get_thread_num();
		for (j = 0; j < T; j++){
			if(my_id == j){
				aux = (tam/T)*(my_id+1);
				/p/rintf("thread %d: %d\n",my_id, i);
				for(i = (tam/T)*(my_id); i < aux; i++){
					vetor[i] = 1;
				}
			}
		}
		#pragma omp barrier
		#pragma single
		{
			vetor[tam/2] = tam;
			for (int i = 0; i < T; i++){
				maiores[i] = vetor[0];
			}
		}
		for (j = 0; j < T; j++){
			if(my_id == j){
				aux = (tam/T)*(my_id+1);
				for(i = (tam/T)*(my_id); i < aux; i++){
					if(vetor[i] > maiores[j])
						maiores[j] = vetor[i];
				}//_
			}
		}
	}
	
	maior = maiores[0];
	for (i = 1; i < T; i++){
		if(maior < maiores[i]){
			maior = maiores[i];
		}
	}
   
	wtime = omp_get_wtime() - wtime;

    printf("PAR REDUCTION: Tam=%d, maior=%d, Elapsed wall clock time = %f  \n", tam, maior, wtime); //Imprime o vetor ordenado
    free(vetor); //Desaloca o vetor lido
	
    return 0;
}