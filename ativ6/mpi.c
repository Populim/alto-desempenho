// Metodologia PCAM
// MPI - Multiplicação de Matrizes - 06 - 05
// Bruno Baldissera Carlotto        10724351
// Bruno Gazoni                       7585037
// Gabriel Eluan Calado           10734453
// João Villaça                10724239
// Matheus Steigenberg Populim  10734710


// Particionamento
//     Primeiramente a alocação das matrizes de entrada é feita sequencialmente, e paralelizamos o preenchimento destas particionando a nível de entrada das matrizes, de forma a termos N² tasks para esta etapa.
// Para a multiplicação de matrizes A x B temos uma complexidade cúbica, onde temos que, para cada entrada em uma linha da matriz A, multiplicamos por uma entrada da coluna correspondente da matriz B, e somamos todos esses resultados parciais para formar uma entrada única da matriz resultante.
//     Dito isso, para alcançarmos uma granulação mais fina queremos aumentar o número de processos paralelos particionando os dados o máximo possível. Então particionamos a computação de cada multiplicação de um elemento Aij por um Bij, resultando em N³ tarefas, N para cada par linha-coluna das matrizes de entrada.

// Comunicação
//     A comunicação começa de forma a tomar os produtos de cada termo das matrizes de entrada obtidos a partir de cada tarefa definida no particionamento que formam parcelas de uma entrada da matriz resultante. Então para cada entrada da matriz resultante fazemos as somas parciais dos elementos descritos anteriormente, de forma a gerar linhas completas da matriz.

// Aglomeração
//     O processo root efetua a alocação e preenchimento das matrizes de entrada e faz um broadcast para (N-1) novos processos, correspondentes às linhas da matriz resultado. Um vetor correspondente a cada linha da matriz A de entrada após receber os dados é combinado com a matriz B via um dos N processos (pois o root está incluso) para gerar uma linha da matriz C.

// Mapeamento
// O número de processos a gerar é limitado pelo MPI em função do número de slots. Os slots determinam a quantidade de processos a escalonar em cada processador e o padrão para o slot é um processo por core, mas pode mudar.


// Compilar:
// mpicc mpi.c -o mpi -fopenmp
// Rodar:
// mpirun -np 8 mpi

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>
#define N 8
#define T 8 


//função para alocar as matrizes A,B,C
double** alocaMatrizDouble(){
	double *data = (double *)malloc(N*N*sizeof(double));
	double **array= (double **)malloc(N*sizeof(double*));
	for (int i=0; i<N; i++)
		array[i] = &(data[N*i]);
	return array;
}

//cria uma matriz aleatória
void preencheMatriz(double** M,int n){
	#pragma omp parallel for num_threads(T)
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			//M[i][j] = (rand()%200)/10.0;
			M[i][j] = 1;
			
		}
	}
}

//imprime uma matriz, com uma casa decimal e sem espaço no final da linha
void imprimeMatriz(double** M,int n){
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n-1; j++){
			printf("%.1lf ",M[i][j]);
		}
		printf("%.1lf",M[i][n-1]);//sem espaço no fim da linha
		printf("\n");
	}
}

//libera matriz
void freeMatriz(double** M){
	free(*M);
	free(M);
}

int main(int argc,char* argv[]){
	srand(0);
	int np, rank, ret=0,status;
	MPI_Init( &argc, &argv );
	MPI_Comm_size(MPI_COMM_WORLD, &np);	

	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("eu sou o proc %d,nro de proc: %d\n",rank, np);

	if(N != np){
		printf("Execute o programa com o número de processos igual a N!\n\n");
		ret = MPI_Finalize();
		return;
	}

	double **A = NULL;
	double **B = NULL;
	B = alocaMatrizDouble();
	double* v;
	double* res;
	v = (double*)malloc(N*sizeof(double));
	res = (double*)malloc(N*sizeof(double));
	for (int i = 0; i < N; i++)
		res[i] = 0;
	A = alocaMatrizDouble();

	// se for o processo pai
	double **C = alocaMatrizDouble();
	double* RESULTADO = (double*) malloc(N*N*sizeof(double));
	if (rank == 0) {
		printf("matrizes A e B:\n");
		preencheMatriz(A,N);
		imprimeMatriz(A,N);
		preencheMatriz(B,N);
		imprimeMatriz(A,N);
		printf("-----------\n");
		printf("\n");
	}

	MPI_Bcast(*B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Scatter(*A,N,MPI_DOUBLE,v,N,MPI_DOUBLE,0,MPI_COMM_WORLD);

	//usleep(1000*rank);
	//printf("%d\n", rank);
	//for (int i = 0; i < N; i++){
	//	printf("%g ", v[i]);		
	//}
	//printf("\n");

	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			res[i] += v[j]*A[i][j];
		}
	}

	MPI_Gather(res, N, MPI_DOUBLE, C[0], N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	//printf("chegou %d\n", rank);

	if (rank == 0) {
		imprimeMatriz(C,N);
		printf("-----------\n");
		printf("\n");
	}


	ret = MPI_Finalize();
	if (ret == MPI_SUCCESS)
		//printf("MPI_Finalize success! \n");
	return 0;
}