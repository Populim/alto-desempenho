// Atividade 6
//
// Alunos:
// Bruno Baldissera Carlotto 	   10724351
// Bruno Gazoni		               7585037
// Gabriel Eluan Calado		   10734453
// João Villaça 			   10724239
// Matheus Steigenberg Populim  10734710
//
// Compilar:
// make all
// Rodar:
// make run
// Rodar com input:
// make run < input.in

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>
#define N 10

#define T 8 //mude aqui o número de threads

//..............//0013.........    ...........

//007...

//função para alocar as matrizes A,B,C,D
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
			M[i][j] = (rand()%200)/10.0;
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
	//define a key geradora dos números aleatórios
	//MPI_Status status;
	srand(0);


	int np, rank, ret=0;
	int errcodes[N];
	MPI_Comm parentcomm, intercomm;
	MPI_Init( &argc, &argv );
	MPI_Comm_size(MPI_COMM_WORLD, &np);	
	MPI_Comm_get_parent( &parentcomm );

	if(np > N){
		printf("O número de processadores não pode ser maior que N!!!\n");
		ret = MPI_Finalize();
		if (ret == MPI_SUCCESS)
			printf("MPI_Finalize success! \n");
	}

	if (parentcomm == MPI_COMM_NULL) {
		MPI_Comm_spawn( "mpi2", MPI_ARGV_NULL, N, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, errcodes );
		printf("I'm the parent.\n");
	}
	else {
		printf("I'm the spawned.\n");
	}

	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("eu sou o proc %d,nro de proc: %d\n",rank, np);



	double **A = NULL;
	double **B = NULL;
	B = alocaMatrizDouble();

	//printf("B[0]: %p\n", B[0]);
	//printf("*B: %p\n", *B);
	//printf(" B: %p\n", B);

	//cria vários processos





	double* v;
	double* res;
	v = (double*)malloc(N*sizeof(double));
	res = (double*)malloc(N*sizeof(double));
	for (int i = 0; i < N; i++)
		res[i] = 0;


	//ouble teste[10][10];
	A = alocaMatrizDouble();

	// se for o processo pai
	if (rank == 0 && parentcomm == MPI_COMM_NULL) {
		double **C = NULL;
		//aloca matrizes
		C = alocaMatrizDouble();
		//aloca matriz que armazenará os resultados:
		//mudar pra faz matriz
		preencheMatriz(A,N);
		imprimeMatriz(A,N);
		preencheMatriz(B,N);
	}

	printf("%p\n", B[0]);
	MPI_Bcast(B[0], N*N, MPI_DOUBLE, 0, intercomm);
	return;
	//MPI_Bcast(teste, 10*10, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	//printf("notsegfault\n");;
	MPI_Scatter(A[0],N,MPI_DOUBLE,v,N,MPI_DOUBLE,0,intercomm);

	sleep(0.1*rank);

	for (int i = 0; i < N; i++){
		printf("%g ", v[i]);		
	}
	printf("\n");

	MPI_Barrier(MPI_COMM_WORLD);



	printf("olá, matriz A no processo %d:\n", rank);
	for (int i = 0; i < N; i++){
		printf("%g ", v[i]);
	}
	printf("\n");
	

	ret = MPI_Finalize();
	if (ret == MPI_SUCCESS)
		printf("MPI_Finalize success! \n");
	return 0;
}