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
	int *vetor, i, maior, tam;

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
	for(i = 0; i < tam; i++)
		vetor[i] = 1;
	
	vetor[tam/2] = tam;

    wtime = omp_get_wtime();

        
	maior = vetor[0];
	#pragma omp parallel for num_threads(T) private(i)
	for(i = 1; i < tam; i++){
		//printf("%d\n", i);
		if(vetor[i] > maior)
			maior = vetor[i];
	}

	/*
	*************************************************************************************
	Não modifique este trecho de código
	*/
   
	wtime = omp_get_wtime() - wtime;

    printf("PAR REDUCTION: Tam=%d, maior=%d, Elapsed wall clock time = %f  \n", tam, maior, wtime); //Imprime o vetor ordenado
    free(vetor); //Desaloca o vetor lido
	
    return 0;
}
