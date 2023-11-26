// Prova 1 de computação de alto desempenho
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
#define T 8 //mude aqui o número de threads

//função para alocar as matrizes A,B,C,D
double** alocaMatrizDouble(double** M,int n){
	M = (double**)malloc(sizeof(double*)*n);
	for (int i = 0; i < n; i++){
		M[i] = (double*)malloc(sizeof(double)*n);
	}
	return M;
}

//função para alocar as matrizes R1 e R2, inicializando-as com 0
double** alocaMatrizDouble_Inicializa(double** M,int n){
	M = (double**)malloc(sizeof(double*)*n);
	for (int i = 0; i < n; i++){
		M[i] = (double*)malloc(sizeof(double)*n);
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			M[i][j] = 0.0;
		}
	}
	return M;
}

//le uma matriz da entrada padrão (STDIN)
void leMatriz(double** M,int n){
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			scanf("%lf",&M[i][j]);
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
void freeMatriz(double** M,int n){
	for (int i = 0; i < n; i++){
		free(M[i]);
	}
	free(M);
}

int main(){
	//tamanho da matriz:
	int n;
	scanf("%d",&n);

	//matriz a serem usadas no código:
	double **A, **B, **C, **D, **R1, **R2;
	
	//aloca matrizes
	A = alocaMatrizDouble(A,n);
	B = alocaMatrizDouble(B,n);
	C = alocaMatrizDouble(C,n);
	D = alocaMatrizDouble(D,n);
	//aloca matriz que armazenará os resultados:
	R1 = alocaMatrizDouble_Inicializa(R1,n);
	R2 = alocaMatrizDouble_Inicializa(R2,n);


	leMatriz(A,n);
	leMatriz(B,n);
	leMatriz(C,n);
	leMatriz(D,n);



	// verifica se as entradas foram lidas corretamente:
	//imprimeMatriz(A,n);
	//imprimeMatriz(B,n);
	//imprimeMatriz(C,n);
	//imprimeMatriz(D,n);


	//double wtime = omp_get_wtime();

	#pragma omp parallel num_threads(T)
	{
		//paraleliza para cada output R1[i][j]
		#pragma omp for collapse(2)
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				for (int k = 0; k < n; k++){
					R1[i][j] += A[i][k]*B[k][j];
				}
			}
		}

		
		#pragma omp for collapse(2)
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				for (int k = 0; k < n; k++){
					R2[i][j] += C[i][k]*D[k][j];
				}
			}
		}

		#pragma omp barrier
		#pragma omp for
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				R1[i][j] += R2[i][j]; 
			}
		}
	}

	//tempo gasto:
	//printf("%lf\n",omp_get_wtime() - wtime);

	//resultado:
	imprimeMatriz(R1,n);
	
	freeMatriz(A,n);
	freeMatriz(B,n);
	freeMatriz(C,n);
	freeMatriz(D,n);
	freeMatriz(R1,n);
	freeMatriz(R2,n);
}