// to compile: gcc 07-01-encontra_max_seq.c -o 07-01-encontra_max_seq -fopenmp
// to execute: 07-01-encontra_max_seq <num_elements>

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main(int argc,char **argv)
{
    int *vetor,i,maior=0,tam;

    double wtime;

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
    for(i=0; i<tam; i++)
    {
		vetor[i] = 1;
    } // fim do for
	// fixando o maior valor no vetor para validacao posterior
    vetor[tam/2] = tam;

    // getting start time after malloc.
    wtime = omp_get_wtime();

    maior = vetor[0];
    for(i = 1; i < tam; i++)
    {
        if(vetor[i]>maior)
		{
            maior=vetor[i];
		} // fim do if
    } // fim do for

    wtime = omp_get_wtime() - wtime;

/*   // imprimindo o vetor para conferencia
    for(i = 0; i < tam; i++)
    {
	printf("vetor[%d]=%d\n",i,vetor[i]);
    }
    printf("\n");
*/    
    printf("Tam=%d, maior=%d\nElapsed wall clock time = %f \n", tam, maior, wtime);
    free(vetor); //Desaloca o vetor lido
    
    return 0;
} // fim da main
