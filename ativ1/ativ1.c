// compilar: make all
// executar: make run tam=12345
// substitua '12345' pelo valor desejado

// Alunos:
// Bruno Baldissera
// Bruno Gazoni
// Gabriel Eluan Calado
// Matheus Steigenberg Populim
// João Nascimento Villaça

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#define T 8

int main(int argc,char **argv){
	/*
		Não modifique este trecho de código
	*************************************************************************************
	*/
	double wtime;
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

	//para substituir a diretiva for, separamos manualmente trechos do loop em T pedaços iguais
	//assim, a variável my_id armazena o id de cada thread e atribui a cada uma seu
	//respectivo trecho

	// cria T threads para executar o código presente nesse escopo
	// i,j,my_id,aux foram declaradas como privadas pois cada thread
	#pragma omp parallel num_threads(T) private(i,j,my_id,aux)
	{
		my_id = omp_get_thread_num();
		for (j = 0; j < T; j++){
			if(my_id == j){
				aux = (tam/T)*(my_id+1);//para que o limite de cada trecho não seja
							// recalculado a cada iteração do loop abaixo,
							// atribuímos-o a uma variável auxiliar
				for(i = (tam/T)*(my_id); i < aux; i++){
					vetor[i] = 1;
				}
			}
		}
		//apenas uma thread é necessária nesse trecho
		#pragma omp single
		{
			vetor[tam/2] = tam;
			for (i = 0; i < T; i++){
				maiores[i] = vetor[0];
			}
		}
		//barrier para esperar e sincronizar as threads
		#pragma omp barrier
		for (j = 0; j < T; j++){
			if(my_id == j){
				aux = (tam/T)*(my_id+1);
				for(i = (tam/T)*(my_id); i < aux; i++){
					if(vetor[i] > maiores[j])
						maiores[j] = vetor[i];
				}
			}
		}
	}
	//como o número de execuções desse trecho depende 
	// do número de threads que é pequeno (8), é mais eficiente
	// executá-lo sequencialmente do que fazer cada thread
	// entrar no for para executar apenas um if.
	
	//após armazenar os maiores números de cada trecho
	// do vetor nas variáveis maiores[i], selecionamos
	// sequencialmente a maior entre estas.
	maior = maiores[0];
	for (i = 1; i < T; i++){
		if(maior < maiores[i]){
			maior = maiores[i];
		}
	}

	//tempo gasto:
	wtime = omp_get_wtime() - wtime;
	printf("PAR REDUCTION: Tam=%d, maior=%d, Elapsed wall clock time = %f  \n", tam, maior, wtime); //Imprime o vetor ordenado
	free(vetor); //Desaloca o vetor lido
	return 0;
}


